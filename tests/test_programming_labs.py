"""Programming package regressions; Python standard library only."""

import copy
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch
import zipfile


SPEC = importlib.util.spec_from_file_location(
    "programming_labs", Path(__file__).resolve().parents[1] / "scripts" / "programming_labs.py"
)
programming = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(programming)


class ProgrammingLabsTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="programming-lab-test-")
        self.workspace = Path(self.temporary.name)
        self.root = self.workspace / "repo"
        self.root.mkdir()
        self.stage = self.workspace / "stage"
        self.messages = []
        self.lab_id = "lab_02_suchen"
        self.lab = self.root / "code" / "labs" / self.lab_id

    def tearDown(self):
        self.temporary.cleanup()

    def write(self, relative, content):
        path = self.root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding="utf-8")
        return path

    def lab_file(self, relative, content):
        return self.write(f"code/labs/{self.lab_id}/{relative}", content)

    def save_manifest(self, document):
        self.lab_file("lab.json", json.dumps(document))

    def fixture(self):
        self.write(f"labs/{self.lab_id}.qmd", "---\ntitle: Suchen\n---\n")
        self.lab_file("README.md", "Start in starter/; run python3 ../tests/check.py.\n")
        self.lab_file("starter/search.py", "def first(values, target):\n    return None  # TODO\n")
        self.lab_file("solution/search.py", "def first(values, target):\n    return next((i for i, item in enumerate(values) if item == target), None)\n")
        self.lab_file("tests/check.py", """import json
from pathlib import Path
import sys
sys.path.insert(0, str(Path.cwd()))
from search import first
cases = [
    ('test_first', [4, 7, 7], 7, 1),
    ('test_zero', [4, 7], 4, 0),
    ('test_notfound', [4], 8, None),
    ('test_empty', [], 8, None),
]
failures = [name for name, values, target, result in cases if first(values, target) != result]
print(json.dumps({'tests': len(cases), 'failures': failures, 'errors': []}))
raise SystemExit(bool(failures))
""")
        self.document = {
            "version": 1,
            "language": "Python",
            "runtime": {"command": [sys.executable, "-c", "print('Python test')"], "output_pattern": "^Python test$"},
            "student_files": ["README.md", "starter/search.py", "tests/check.py"],
            "solution_files": ["solution/search.py"],
            "checks": [
                {"name": "import", "command": [sys.executable, "-c", "import search"], "starter": {"exit_code": 0}, "solution": {"exit_code": 0}},
                {"name": "tests", "command": [sys.executable, "../tests/check.py"],
                 "starter": {"exit_code": 1, "json": {"tests": 4, "failures": ["test_first", "test_zero"], "errors": []}},
                 "solution": {"exit_code": 0, "json": {"tests": 4, "failures": [], "errors": []}}},
            ],
        }
        self.save_manifest(self.document)

    def build(self):
        return programming.build_programming_packages(self.root, self.stage, self.messages.append)

    def test_missing_manifests_are_an_explicit_noop(self):
        self.assertEqual(self.build(), {})
        self.assertIn("no code/labs/*/lab.json", self.messages[0])
        self.assertFalse(self.stage.exists())

    def test_exact_starter_failures_and_reference_solution_are_verified(self):
        self.fixture()
        packages = self.build()
        self.assertEqual(set(packages), {f"_site/labs/{self.lab_id}-startcode.zip"})
        self.assertTrue(next(iter(packages.values())).is_file())
        self.assertIn("starter, reference solution and extracted ZIP verified", self.messages[0])
        self.assertFalse((self.root / "_site").exists())

    def test_archive_is_allowlisted_and_source_files_are_unchanged(self):
        self.fixture()
        for relative in ("solution/private-answer.txt", "starter/__pycache__/search.pyc", "starter/build/output.txt", ".git/config", "notes.txt"):
            self.lab_file(relative, "must not be delivered\n")
        original_check = (self.lab / "tests/check.py").read_text()
        self.lab_file("tests/check.py", "from pathlib import Path\nPath('build').mkdir(exist_ok=True)\nPath('build/generated.txt').write_text('byproduct')\n" + original_check)
        source_files = [path for path in self.lab.rglob("*") if path.is_file()]
        original = {path.relative_to(self.lab): (path.read_bytes(), path.stat().st_mode) for path in source_files}
        for path in source_files:
            path.chmod(0o444)
        expected_modes = {path.relative_to(self.lab): path.stat().st_mode for path in source_files}
        packages = self.build()
        with zipfile.ZipFile(next(iter(packages.values()))) as archive:
            self.assertEqual(set(archive.namelist()), {f"{self.lab_id}/{relative}" for relative in self.document["student_files"]})
            self.assertEqual(archive.read(f"{self.lab_id}/starter/search.py"), original[Path("starter/search.py")][0])
        self.assertEqual({path.relative_to(self.lab) for path in self.lab.rglob("*") if path.is_file()}, set(original))
        for relative, (content, _) in original.items():
            self.assertEqual((self.lab / relative).read_bytes(), content)
            self.assertEqual((self.lab / relative).stat().st_mode, expected_modes[relative])

    def test_starter_and_extracted_zip_cannot_see_reference_solution(self):
        self.fixture()
        absence = {"name": "isolated", "command": [sys.executable, "-c", "from pathlib import Path; assert not (Path.cwd().parent / 'solution').exists()"], "starter": {"exit_code": 0}, "solution": {"exit_code": 0}}
        # The solution check uses the same command, so restrict the assertion to starter/.
        absence["command"][-1] = "from pathlib import Path; here = Path.cwd(); assert here.name != 'starter' or not (here.parent / 'solution').exists(); assert 'extracted' not in here.parts or not any(parent.joinpath('reference-check').exists() for parent in here.parents)"
        self.document["checks"].insert(0, absence)
        self.save_manifest(self.document)
        self.build()

    def test_actual_extracted_archive_is_checked(self):
        self.fixture()
        original_archive = programming._archive

        def damaged_archive(source, files, lab_id, destination):
            original_archive(source, files, lab_id, destination)
            with zipfile.ZipFile(destination) as archive:
                entries = [(entry, archive.read(entry)) for entry in archive.infolist()]
            with zipfile.ZipFile(destination, "w") as archive:
                for entry, content in entries:
                    if entry.filename.endswith("starter/search.py"):
                        content = b"raise RuntimeError('broken packaged starter')\n"
                    archive.writestr(entry, content)

        with patch.object(programming, "_archive", damaged_archive):
            with self.assertRaisesRegex(programming.ProgrammingLabError, "extracted ZIP starter import"):
                self.build()
        self.assertEqual(list(self.stage.iterdir()), [])

    def test_wrong_starter_failure_ids_and_unexpected_errors_are_rejected(self):
        self.fixture()
        for result in (
            {"tests": 4, "failures": ["test_first"], "errors": []},
            {"tests": 4, "failures": ["test_first", "test_zero", "test_empty"], "errors": []},
            {"tests": 4, "failures": ["test_first", "test_zero"], "errors": ["test_notfound"]},
            {"tests": True, "failures": ["test_first", "test_zero"], "errors": []},
        ):
            with self.subTest(result=result):
                self.document["checks"][1]["starter"]["json"] = result
                self.save_manifest(self.document)
                with self.assertRaisesRegex(programming.ProgrammingLabError, "expected exactly"):
                    self.build()

    def test_crash_with_same_exit_code_as_todo_failures_is_rejected(self):
        self.fixture()
        self.lab_file("tests/check.py", "raise RuntimeError('unexpected crash')\n")
        with self.assertRaisesRegex(programming.ProgrammingLabError, "invalid JSON"):
            self.build()

    def test_reference_solution_must_actually_pass(self):
        self.fixture()
        self.lab_file("solution/search.py", "def first(values, target):\n    return None\n")
        with self.assertRaisesRegex(programming.ProgrammingLabError, "solution tests: exit 1, expected 0"):
            self.build()

    def test_runtime_fullmatch_checks_combined_output_and_status(self):
        self.fixture()
        for command in (
            [sys.executable, "-c", "print('prefix Python test suffix')"],
            [sys.executable, "-c", "print('Python test'); raise SystemExit(1)"],
        ):
            with self.subTest(command=command):
                self.document["runtime"]["command"] = command
                self.save_manifest(self.document)
                with self.assertRaisesRegex(programming.ProgrammingLabError, "runtime mismatch"):
                    self.build()
        self.document["runtime"]["command"] = [sys.executable, "-c", "import sys; print('Python test', file=sys.stderr)"]
        self.save_manifest(self.document)
        self.build()

    def test_missing_matching_worksheet_or_declared_source_is_rejected(self):
        self.fixture()
        worksheet = self.root / "labs" / f"{self.lab_id}.qmd"
        worksheet.unlink()
        with self.assertRaisesRegex(programming.ProgrammingLabError, "missing matching worksheet"):
            self.build()
        worksheet.write_text("worksheet", encoding="utf-8")
        (self.lab / "solution/search.py").unlink()
        with self.assertRaisesRegex(programming.ProgrammingLabError, "missing regular source file"):
            self.build()
        self.assertFalse(self.stage.exists())

    def test_unrenderable_worksheet_ids_and_nested_only_pairings_are_rejected(self):
        self.fixture()
        worksheet = self.root / "labs" / f"{self.lab_id}.qmd"
        for lab_id in ("foo", "lab_1_suchen", "lab_001_suchen", "lab_02_", "_lab_02_suchen", ".lab_02_suchen", "lab_02_template", "lab_02_TemPlate_suchen", "probeklausur"):
            with self.subTest(lab_id=lab_id):
                renamed_lab = self.lab.with_name(lab_id)
                renamed_worksheet = worksheet.with_name(f"{lab_id}.qmd")
                self.lab.rename(renamed_lab)
                worksheet.rename(renamed_worksheet)
                try:
                    with self.assertRaisesRegex(programming.ProgrammingLabError, "renderable lab_NN_thema worksheet"):
                        self.build()
                finally:
                    renamed_lab.rename(self.lab)
                    renamed_worksheet.rename(worksheet)
        nested = worksheet.parent / "nested" / worksheet.name
        nested.parent.mkdir()
        worksheet.rename(nested)
        with self.assertRaisesRegex(programming.ProgrammingLabError, "missing matching worksheet"):
            self.build()
        self.assertFalse(self.stage.exists())

    def test_expected_nonzero_requires_json_and_solution_requires_success(self):
        self.fixture()
        del self.document["checks"][1]["starter"]["json"]
        self.save_manifest(self.document)
        with self.assertRaisesRegex(programming.ProgrammingLabError, "expected failure requires an exact JSON"):
            self.build()
        self.document["checks"][1]["starter"]["json"] = {}
        self.document["checks"][1]["solution"]["exit_code"] = 1
        self.save_manifest(self.document)
        with self.assertRaisesRegex(programming.ProgrammingLabError, "reference solution must pass"):
            self.build()

    def test_path_traversal_hidden_solution_and_generated_files_are_rejected(self):
        self.fixture()
        original = copy.deepcopy(self.document)
        for forbidden in ("../outside.py", "/outside.py", "starter/../solution/search.py", "starter\\search.py", "C:/outside.py", ".git/config", "starter/.env", "solution/search.py", "starter/solution/answer.py", "starter/__pycache__/search.pyc", "starter/build/object", "starter/search.pyc"):
            with self.subTest(path=forbidden):
                self.document = copy.deepcopy(original)
                self.document["student_files"].append(forbidden)
                self.save_manifest(self.document)
                with self.assertRaises(programming.ProgrammingLabError):
                    self.build()

    def test_source_and_directory_symlinks_are_rejected(self):
        self.fixture()
        source = self.lab / "starter/search.py"
        source.unlink()
        outside = self.workspace / "outside.py"
        outside.write_text("external", encoding="utf-8")
        source.symlink_to(outside)
        with self.assertRaisesRegex(programming.ProgrammingLabError, "symlinks are not allowed"):
            self.build()
        source.unlink()
        source.write_text("source", encoding="utf-8")
        directory = self.lab / "starter"
        directory.rename(self.lab / "moved-starter")
        directory.symlink_to(self.lab / "moved-starter", target_is_directory=True)
        with self.assertRaisesRegex(programming.ProgrammingLabError, "symlinks are not allowed"):
            self.build()

    def test_invalid_manifest_fields_types_and_duplicate_paths_are_rejected(self):
        self.fixture()
        original = copy.deepcopy(self.document)
        mutations = [
            lambda d: d.update(version=True),
            lambda d: d.update(checks=[]),
            lambda d: d.update(language=""),
            lambda d: d["runtime"].update(output_pattern="["),
            lambda d: d["runtime"].update(command="python3 --version"),
            lambda d: d["checks"][0].update(commnad=["python3"]),
            lambda d: d["checks"].append(copy.deepcopy(d["checks"][0])),
            lambda d: d["checks"][0]["starter"].update(exit_code=True),
            lambda d: d["checks"][0]["starter"].update(json=[]),
            lambda d: d["student_files"].append("starter/SEARCH.py"),
            lambda d: d["student_files"].remove("README.md"),
            lambda d: d.update(solution_files=["starter/search.py"]),
        ]
        for mutate in mutations:
            with self.subTest(mutation=mutate):
                document = copy.deepcopy(original)
                mutate(document)
                self.save_manifest(document)
                with self.assertRaises(programming.ProgrammingLabError):
                    self.build()

    def test_duplicate_json_keys_and_case_colliding_outputs_are_rejected(self):
        self.fixture()
        self.lab_file("lab.json", '{"version": 1, "version": 1}')
        with self.assertRaisesRegex(programming.ProgrammingLabError, "duplicate JSON key"):
            self.build()
        self.save_manifest(self.document)
        second_id = "lab_02_SUCHEN"
        second = self.root / "code/labs" / second_id
        second.mkdir()
        (second / "lab.json").write_text(json.dumps(self.document), encoding="utf-8")
        self.write(f"labs/{second_id}.qmd", "worksheet")
        # Ensure the colliding lab is validated first regardless of sort order.
        for relative in self.document["student_files"] + self.document["solution_files"]:
            destination = second / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_bytes((self.lab / relative).read_bytes())
        with self.assertRaisesRegex(programming.ProgrammingLabError, "duplicate public output"):
            self.build()

    def test_timeout_and_missing_runtime_are_actionable_errors(self):
        self.fixture()
        with patch.object(programming.subprocess, "run", side_effect=subprocess.TimeoutExpired(["python"], 60)):
            with self.assertRaisesRegex(programming.ProgrammingLabError, "command could not complete"):
                self.build()
        self.document["runtime"]["command"] = ["does-not-exist-for-programming-lab-test"]
        self.save_manifest(self.document)
        with self.assertRaisesRegex(programming.ProgrammingLabError, "command could not complete"):
            self.build()

    def test_command_arrays_preserve_empty_arguments_without_a_shell(self):
        self.fixture()
        self.document["checks"].append({
            "name": "arguments",
            "command": [sys.executable, "-c", "import sys; assert sys.argv[1:] == ['', '$(exit 8)']", "", "$(exit 8)"],
            "starter": {"exit_code": 0}, "solution": {"exit_code": 0},
        })
        self.save_manifest(self.document)
        self.build()

    def test_staging_rejects_source_publication_directories_and_symlinks(self):
        self.fixture()
        for relative in (".", "code/stage", "labs/stage", "_site/stage"):
            with self.subTest(stage=relative):
                with self.assertRaisesRegex(programming.ProgrammingLabError, "staging must be outside"):
                    programming.build_programming_packages(self.root, self.root / relative)
        alias = self.workspace / "stage-alias"
        self.stage.mkdir()
        alias.symlink_to(self.stage, target_is_directory=True)
        with self.assertRaisesRegex(programming.ProgrammingLabError, "symlinks are not allowed"):
            programming.build_programming_packages(self.root, alias)

    def test_inherited_pythonpath_cannot_supply_missing_student_dependencies(self):
        self.fixture()
        outside = self.workspace / "external-import"
        outside.mkdir()
        (outside / "private_dependency.py").write_text("value = 1", encoding="utf-8")
        self.lab_file("starter/search.py", "import private_dependency\n")
        with patch.dict(os.environ, {"PYTHONPATH": str(outside)}):
            with self.assertRaisesRegex(programming.ProgrammingLabError, "starter import: exit"):
                self.build()


if __name__ == "__main__":
    unittest.main()
