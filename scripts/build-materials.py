#!/usr/bin/env python3
"""Build teaching materials and verified starter packages locally and in CI."""

from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

# Also support importing this CLI by file path in the orchestration tests.
sys.path.insert(0, str(Path(__file__).resolve().parent))
from programming_labs import ProgrammingLabError, build_programming_packages


LAB_SOURCE = re.compile(r"(?:lab_[0-9]{2}_.+|probeklausur)\.qmd\Z")
LAB_PDF = re.compile(r"(?:lab_[0-9]{2}_.+|probeklausur(?:-musterloesung|-studierende)?)\.pdf\Z")
CATEGORIES = ("website", "handouts", "labs", "code")


class BuildError(RuntimeError):
    """A required build step or its expected artifact failed."""


def add_artifact(files, destination: Path, source: Path):
    path = destination.as_posix()
    if path in files:
        raise BuildError(f"Mehrere Quellen erzeugen denselben Ausgabepfad: {path}")
    files[path] = source


def is_source(path: Path) -> bool:
    """Ignore include/cache directories and documents explicitly named as templates."""
    return not any(
        part.startswith((".", "_")) or "template" in part.lower()
        for part in path.parts
    )


def discover_sources(root: Path) -> dict[str, list[Path]]:
    lectures = sorted(
        path for path in (root / "vorlesungen").rglob("*.qmd")
        if is_source(path.relative_to(root))
    )
    pages = sorted(
        path for path in root.glob("*.qmd")
        if is_source(path.relative_to(root))
    )
    labs = sorted(
        path for path in (root / "labs").rglob("*.qmd")
        if LAB_SOURCE.fullmatch(path.name) and is_source(path.relative_to(root))
    )
    return {"website": pages + lectures, "handouts": lectures, "labs": labs}


def is_material_resource(path: Path) -> bool:
    """Discard source-side material outputs copied as linked Quarto resources."""
    return (path.suffix == ".pdf" and (
        path.parts[0] == "labs" and bool(LAB_PDF.fullmatch(path.name))
        or path.parts[0] == "vorlesungen" and path.name.endswith("-handout.pdf")
    )) or (
        path.parts[0] == "labs" and path.name.endswith("-startcode.zip")
    ) or path.parts[:2] == ("code", "labs")


def owned_path(path: str) -> bool:
    candidate = Path(path)
    return (
        not candidate.is_absolute()
        and ".." not in candidate.parts
        and len(candidate.parts) > 1
        and (
            candidate.parts[0] in ("_site", "_handout")
            or candidate.parts[:2] == ("labs", "_output")
        )
    )


class MaterialBuilder:
    def __init__(self, root: Path, runner=subprocess.run, report=print):
        self.root = root.resolve()
        self.runner = runner
        self.report = report
        self.state = self.root / ".quarto" / "materials-build"
        self.manifest = self.state / "manifest.json"
        # Profiles inherited from a shell must not expose solutions in student PDFs.
        self.env = os.environ.copy()
        self.env.pop("QUARTO_PROFILE", None)

    def render(self, source: Path, project: Path, output: Path, profile=None, pdf=False):
        command = ["quarto", "render", source.relative_to(project).as_posix()]
        command += ["--output-dir", str(output), "--no-clean"]
        if pdf:
            command += ["--to", "pdf", "-M", "latex-auto-install:false"]
        if profile:
            command += ["--profile", profile]
        label = profile or ("Studierende" if pdf else "Website")
        self.report(f"[{label}] {source.relative_to(self.root).as_posix()}")
        try:
            self.runner(command, cwd=project, env=self.env, check=True)
        except (OSError, subprocess.CalledProcessError) as error:
            raise BuildError(f"Render fehlgeschlagen: {source.relative_to(self.root)} ({label}): {error}") from error
        expected = output / source.relative_to(project).with_suffix(".pdf" if pdf else ".html")
        if not expected.is_file() or expected.stat().st_size == 0:
            raise BuildError(f"Erwartete Ausgabe fehlt oder ist leer: {expected}")
        return expected

    def load_manifest(self):
        if not self.manifest.exists():
            return {}
        try:
            data = json.loads(self.manifest.read_text(encoding="utf-8"))
            if not isinstance(data, dict) or any(
                category not in CATEGORIES or not isinstance(paths, list)
                or any(not isinstance(path, str) or not owned_path(path) for path in paths)
                for category, paths in data.items()
            ):
                raise ValueError("ungültige Dateiliste")
            return data
        except (OSError, ValueError) as error:
            raise BuildError(f"Build-Manifest kann nicht gelesen werden: {self.manifest}: {error}") from error

    def publish(self, artifacts, previous):
        """Replace only generated paths; remove stale files recorded by this builder."""
        updated = dict(previous)
        for category, files in artifacts.items():
            for destination, source in files.items():
                target = self.root / destination
                target.parent.mkdir(parents=True, exist_ok=True)
                with tempfile.NamedTemporaryFile(dir=target.parent, prefix=".materials-", delete=False) as temporary:
                    temporary_path = Path(temporary.name)
                try:
                    shutil.copyfile(source, temporary_path)
                    os.replace(temporary_path, target)
                finally:
                    temporary_path.unlink(missing_ok=True)
            for stale in set(previous.get(category, [])) - files.keys():
                (self.root / stale).unlink(missing_ok=True)
            updated[category] = sorted(files)
        temporary_manifest = self.manifest.with_suffix(".tmp")
        temporary_manifest.write_text(json.dumps(updated, indent=2) + "\n", encoding="utf-8")
        temporary_manifest.replace(self.manifest)

    def build(self, command: str):
        selected = CATEGORIES if command == "all" else ("labs", "code") if command == "labs" else (command,)
        sources = discover_sources(self.root)
        if "website" in selected and not sources["website"]:
            raise BuildError("Website: keine .qmd-Quellen gefunden; der Pflicht-Build ist nicht möglich.")
        previous = self.load_manifest()
        self.state.mkdir(parents=True, exist_ok=True)
        artifacts = {category: {} for category in selected}
        # A failed render never publishes earlier files from this run or stale PDFs.
        with tempfile.TemporaryDirectory(prefix="run-", dir=self.state) as directory:
            stage = Path(directory)
            for category in selected:
                if category == "code":
                    try:
                        packages = build_programming_packages(self.root, stage / "code", report=self.report)
                    except ProgrammingLabError as error:
                        raise BuildError(f"Programmieraufgaben: {error}") from error
                    for destination, archive in packages.items():
                        add_artifact(artifacts[category], Path(destination), archive)
                    continue
                self.report(f"{category}: {len(sources[category])} Quelle(n).")
                if not sources[category]:
                    self.report(f"{category}: keine passenden Quellen; optionaler Bereich wird übersprungen.")
                if category == "website":
                    output = stage / "website"
                    for source in sources[category]:
                        self.render(source, self.root, output)
                    for source in output.rglob("*"):
                        relative = source.relative_to(output)
                        if source.is_file() and not is_material_resource(relative):
                            add_artifact(artifacts[category], Path("_site") / relative, source)
                elif category == "handouts":
                    for index, source in enumerate(sources[category]):
                        pdf = self.render(source, self.root, stage / "handouts" / str(index), "handout", pdf=True)
                        relative = source.relative_to(self.root).with_suffix(".pdf")
                        add_artifact(artifacts[category], Path("_handout") / relative, pdf)
                        public = relative.with_name(relative.stem + "-handout.pdf")
                        add_artifact(artifacts[category], Path("_site") / public, pdf)
                else:
                    for index, source in enumerate(sources[category]):
                        relative = source.relative_to(self.root / "labs").with_suffix(".pdf")
                        for profile, label, suffix in ((None, "student", ""), ("solution", "solution", "-musterloesung")):
                            pdf = self.render(source, self.root / "labs", stage / "labs" / label / str(index), profile, pdf=True)
                            public = relative.with_name(relative.stem + suffix + ".pdf")
                            add_artifact(artifacts[category], Path("_site/labs") / public, pdf)
                            compatibility = relative.with_name(relative.stem + (suffix or "-studierende") + ".pdf")
                            add_artifact(artifacts[category], Path("labs/_output") / compatibility, pdf)
            self.publish(artifacts, previous)
        self.report("Build erfolgreich. Veröffentlichbare Materialien: _site/")


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("all",) + CATEGORIES, nargs="?", default="all")
    args = parser.parse_args(argv)
    try:
        MaterialBuilder(Path(__file__).resolve().parents[1]).build(args.command)
    except (BuildError, OSError) as error:
        print(f"Build fehlgeschlagen: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
