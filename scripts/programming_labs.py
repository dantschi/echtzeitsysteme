"""Validate programming labs and stage allowlisted, independently tested ZIPs.

Commands in lab.json are trusted repository code, executed without a shell in
temporary copies. This isolates normal compiler/test byproducts from the source;
it is not a security sandbox for untrusted programs.
"""

from __future__ import annotations

import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import stat
import subprocess
import tempfile
from typing import Callable
import zipfile


COMMAND_TIMEOUT = 60
_ARTIFACT_DIRECTORIES = {
    "__pycache__", "build", "dist", "target", "node_modules", "_site",
    "_handout", "_output", "cmakefiles", "coverage", "htmlcov",
}
_ARTIFACT_SUFFIXES = {".pyc", ".pyo", ".o", ".obj", ".a", ".so", ".dll", ".exe", ".class"}


class ProgrammingLabError(RuntimeError):
    """A manifest, exercise, reference solution or packaged starter is invalid."""


def _error(context: str, message: str) -> None:
    raise ProgrammingLabError(f"{context}: {message}")


def _object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise ValueError(f"duplicate JSON key {key!r}")
        result[key] = value
    return result


def _parse_json(value: str, context: str):
    def invalid_constant(constant):
        raise ValueError(f"invalid JSON constant {constant}")

    try:
        return json.loads(value, object_pairs_hook=_object, parse_constant=invalid_constant)
    except (ValueError, TypeError) as exc:
        _error(context, f"invalid JSON: {exc}")


def _fields(value, required: set[str], optional: set[str], context: str) -> None:
    if not isinstance(value, dict):
        _error(context, "must be a JSON object")
    missing = required - value.keys()
    extra = value.keys() - required - optional
    if missing or extra:
        _error(context, f"invalid fields (missing: {sorted(missing)}, unknown: {sorted(extra)})")


def _command(value, context: str) -> list[str]:
    if not isinstance(value, list) or not value or any(
        not isinstance(part, str) or "\x00" in part for part in value
    ) or not value[0]:
        _error(context, "command must be an array of strings with a nonempty executable")
    return value


def _no_symlinks(base: Path, path: Path, context: str) -> None:
    try:
        relative = path.relative_to(base)
    except ValueError:
        _error(context, f"path escapes {base}")
    current = base
    for part in relative.parts:
        current = current / part
        if current.is_symlink():
            _error(context, f"symlinks are not allowed: {current}")


def _source(base: Path, relative: str, context: str) -> Path:
    path = base / relative
    _no_symlinks(base, path, context)
    if not path.is_file():
        _error(context, f"missing regular source file: {relative}")
    if not path.resolve().is_relative_to(base.resolve()):
        _error(context, f"source path escapes the lab: {relative}")
    return path


def _files(value, variant: str, context: str) -> list[str]:
    if not isinstance(value, list) or not value:
        _error(context, "must be a nonempty array of file paths")
    seen = set()
    for relative in value:
        if not isinstance(relative, str) or not relative or "\\" in relative or ":" in relative:
            _error(context, f"invalid relative POSIX file path: {relative!r}")
        parts = relative.split("/")
        if any(not part or part in {".", ".."} or part.startswith(".") for part in parts):
            _error(context, f"unsafe or hidden file path: {relative!r}")
        if PurePosixPath(relative).is_absolute() or "\x00" in relative:
            _error(context, f"unsafe file path: {relative!r}")
        if relative.casefold() in seen:
            _error(context, f"duplicate file path: {relative}")
        seen.add(relative.casefold())
        if any(part.casefold() in _ARTIFACT_DIRECTORIES for part in parts):
            _error(context, f"cache/build files are not sources: {relative}")
        if Path(relative).suffix.casefold() in _ARTIFACT_SUFFIXES:
            _error(context, f"compiled artifacts are not sources: {relative}")
        if variant == "student" and ("solution" in [p.casefold() for p in parts] or relative == "lab.json"):
            _error(context, f"instructor-only file in student_files: {relative}")
        if variant == "solution" and (len(parts) < 2 or parts[0] != "solution"):
            _error(context, f"reference files must be below solution/: {relative}")
    return value


def _manifest(path: Path) -> dict:
    context = str(path)
    try:
        document = _parse_json(path.read_text(encoding="utf-8"), context)
    except (OSError, UnicodeError) as exc:
        _error(context, f"cannot read manifest: {exc}")
    _fields(document, {"version", "language", "runtime", "student_files", "solution_files", "checks"}, set(), context)
    if type(document["version"]) is not int or document["version"] != 1:
        _error(context, "only manifest version 1 is supported")
    if not isinstance(document["language"], str) or not document["language"].strip():
        _error(context, "language must be a nonempty string")
    runtime = document["runtime"]
    _fields(runtime, {"command", "output_pattern"}, set(), f"{context} runtime")
    _command(runtime["command"], f"{context} runtime")
    if not isinstance(runtime["output_pattern"], str) or not runtime["output_pattern"]:
        _error(context, "runtime.output_pattern must be a nonempty regex")
    try:
        re.compile(runtime["output_pattern"])
    except re.error as exc:
        _error(context, f"invalid runtime.output_pattern: {exc}")
    student = _files(document["student_files"], "student", f"{context} student_files")
    _files(document["solution_files"], "solution", f"{context} solution_files")
    if "README.md" not in student or not any(p.startswith("starter/") for p in student) or not any(p.startswith("tests/") for p in student):
        _error(context, "student_files must include README.md and files below starter/ and tests/")
    checks = document["checks"]
    if not isinstance(checks, list) or not checks:
        _error(context, "checks must be a nonempty array")
    names = set()
    for check in checks:
        _fields(check, {"name", "command", "starter", "solution"}, set(), f"{context} check")
        name = check["name"]
        if not isinstance(name, str) or not name.strip() or name in names:
            _error(context, "check names must be nonempty and unique")
        names.add(name)
        _command(check["command"], f"{context} check {name}")
        for variant in ("starter", "solution"):
            expected = check[variant]
            check_context = f"{context} check {name} {variant}"
            _fields(expected, {"exit_code"}, {"json"}, check_context)
            code = expected["exit_code"]
            if type(code) is not int or not 0 <= code <= 255:
                _error(check_context, "exit_code must be an integer from 0 to 255")
            if variant == "solution" and code != 0:
                _error(check_context, "the reference solution must pass (exit_code 0)")
            if code != 0 and "json" not in expected:
                _error(check_context, "an expected failure requires an exact JSON expectation")
            if "json" in expected and not isinstance(expected["json"], dict):
                _error(check_context, "json must be an explicit JSON object")
    return document


def _run(command: list[str], cwd: Path, context: str) -> subprocess.CompletedProcess:
    environment = os.environ.copy()
    # A developer's Python import path must not make a broken ZIP work by accident.
    environment.pop("PYTHONPATH", None)
    environment.pop("PYTHONHOME", None)
    try:
        return subprocess.run(
            command, cwd=cwd, env=environment, capture_output=True, text=True,
            encoding="utf-8", errors="replace", timeout=COMMAND_TIMEOUT, check=False,
        )
    except (OSError, subprocess.TimeoutExpired) as exc:
        _error(context, f"command could not complete: {exc}")


def _same_json(actual, expected) -> bool:
    if type(actual) is not type(expected):
        return False
    if isinstance(expected, dict):
        return actual.keys() == expected.keys() and all(_same_json(actual[key], value) for key, value in expected.items())
    if isinstance(expected, list):
        return len(actual) == len(expected) and all(_same_json(a, b) for a, b in zip(actual, expected))
    return actual == expected


def _check_runtime(document: dict, cwd: Path, context: str) -> str:
    runtime = document["runtime"]
    result = _run(runtime["command"], cwd, f"{context} runtime")
    output = (result.stdout + result.stderr).strip()
    if result.returncode != 0 or re.fullmatch(runtime["output_pattern"], output) is None:
        _error(context, f"runtime mismatch (exit {result.returncode}): {output[:2000]!r}; expected {runtime['output_pattern']!r}")
    return output


def _check_variant(document: dict, cwd: Path, variant: str, context: str) -> None:
    for check in document["checks"]:
        check_context = f"{context} {check['name']}"
        result = _run(check["command"], cwd, check_context)
        expected = check[variant]
        if result.returncode != expected["exit_code"]:
            _error(check_context, f"exit {result.returncode}, expected {expected['exit_code']}; stdout={result.stdout[:2000]!r}, stderr={result.stderr[:2000]!r}")
        if "json" in expected:
            actual = _parse_json(result.stdout, check_context)
            if not _same_json(actual, expected["json"]):
                _error(check_context, f"JSON result {actual!r}, expected exactly {expected['json']!r}")


def _copy_files(source: Path, files: list[str], destination: Path) -> None:
    for relative in files:
        original = _source(source, relative, str(source))
        target = destination / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(original, target)
        target.chmod(0o755 if original.stat().st_mode & 0o111 else 0o644)


def _archive(source: Path, files: list[str], lab_id: str, destination: Path) -> None:
    destination.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(destination, "w", compression=zipfile.ZIP_DEFLATED) as archive:
        for relative in sorted(files):
            original = _source(source, relative, str(source))
            entry = zipfile.ZipInfo(f"{lab_id}/{relative}", date_time=(1980, 1, 1, 0, 0, 0))
            entry.create_system = 3
            mode = 0o755 if original.stat().st_mode & 0o111 else 0o644
            entry.external_attr = (stat.S_IFREG | mode) << 16
            entry.compress_type = zipfile.ZIP_DEFLATED
            archive.writestr(entry, original.read_bytes())


def _extract(archive_path: Path, destination: Path, expected_names: set[str]) -> None:
    with zipfile.ZipFile(archive_path) as archive:
        if set(archive.namelist()) != expected_names or len(archive.namelist()) != len(expected_names):
            _error(str(archive_path), "archive entries do not match the student file allowlist")
        if archive.testzip() is not None:
            _error(str(archive_path), "archive failed its CRC check")
        for entry in archive.infolist():
            # Names were validated against our exact, safe manifest allowlist.
            target = destination / entry.filename
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(archive.read(entry))
            target.chmod((entry.external_attr >> 16) & 0o777)


def build_programming_packages(root: Path, stage: Path, report: Callable[[str], None] = print) -> dict[str, Path]:
    """Return public-relative destinations mapped to validated staged ZIP files.

    The caller owns publication. Source files and existing _site outputs are never
    written. Only root-level labs/<lab_id>.qmd pairings are currently supported.
    """
    root = Path(root).resolve()
    stage = Path(stage).absolute()
    _no_symlinks(Path(stage.anchor), stage, "programming-lab staging")
    stage = stage.resolve()
    if stage == root or any(stage.is_relative_to(root / part) for part in ("code", "labs", "_site")):
        _error(str(stage), "staging must be outside source and published material directories")
    source_root = root / "code" / "labs"
    _no_symlinks(root, source_root, "programming labs")
    if source_root.exists() and not source_root.is_dir():
        _error(str(source_root), "expected a directory")
    manifests = sorted(source_root.glob("*/lab.json")) if source_root.exists() else []
    if not manifests:
        report("Programming labs: no code/labs/*/lab.json manifests; no startcode ZIPs to build.")
        return {}
    labs = []
    outputs = set()
    for manifest in manifests:
        _no_symlinks(root, manifest, str(manifest))
        lab_id = manifest.parent.name
        # Keep the ZIP pairing within build-materials.py's numbered worksheet
        # selection; template worksheets are deliberately excluded there.
        if re.fullmatch(r"lab_[0-9]{2}_[A-Za-z0-9_-]+", lab_id) is None or "template" in lab_id.casefold():
            _error(str(manifest), "lab ID must name a renderable lab_NN_thema worksheet (ASCII letters, digits, underscores and hyphens; no 'template')")
        public_path = f"_site/labs/{lab_id}-startcode.zip"
        if public_path.casefold() in outputs:
            _error(str(manifest), f"duplicate public output: {public_path}")
        outputs.add(public_path.casefold())
        qmd = root / "labs" / f"{lab_id}.qmd"
        _no_symlinks(root, qmd, str(manifest))
        if not qmd.is_file():
            _error(str(manifest), f"missing matching worksheet: labs/{lab_id}.qmd")
        document = _manifest(manifest)
        for relative in document["student_files"] + document["solution_files"]:
            _source(manifest.parent, relative, str(manifest))
        labs.append((manifest.parent, lab_id, public_path, document))
    stage.mkdir(parents=True, exist_ok=True)
    run = Path(tempfile.mkdtemp(prefix="programming-labs-", dir=stage))
    packages = {}
    try:
        for source, lab_id, public_path, document in labs:
            work = run / lab_id
            starter = work / "starter-check"
            reference = work / "reference-check"
            _copy_files(source, document["student_files"], starter)
            _copy_files(source, document["student_files"] + document["solution_files"], reference)
            runtime = _check_runtime(document, starter / "starter", lab_id)
            _check_variant(document, starter / "starter", "starter", f"{lab_id} starter")
            _check_variant(document, reference / "solution", "solution", f"{lab_id} solution")
            shutil.rmtree(starter)
            shutil.rmtree(reference)
            archive = work / f"{lab_id}-startcode.zip"
            _archive(source, document["student_files"], lab_id, archive)
            extracted = work / "extracted"
            expected_names = {f"{lab_id}/{relative}" for relative in document["student_files"]}
            _extract(archive, extracted, expected_names)
            extracted_starter = extracted / lab_id / "starter"
            _check_runtime(document, extracted_starter, f"{lab_id} extracted ZIP")
            _check_variant(document, extracted_starter, "starter", f"{lab_id} extracted ZIP starter")
            packages[public_path] = archive
            report(f"Programming lab {lab_id}: {runtime}; starter, reference solution and extracted ZIP verified ({len(expected_names)} student files).")
    except Exception:
        shutil.rmtree(run)
        raise
    return packages
