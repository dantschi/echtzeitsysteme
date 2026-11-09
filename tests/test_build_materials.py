"""Build orchestration regressions; no Quarto, TeX, Node or network required."""

import contextlib
import importlib.util
import io
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch


SPEC = importlib.util.spec_from_file_location(
    "build_materials", Path(__file__).resolve().parents[1] / "scripts" / "build-materials.py"
)
materials = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(materials)


class FakeQuarto:
    def __init__(self, fail=None, omit=None, copy_stale=False):
        self.calls = []
        self.fail = fail
        self.omit = omit
        self.copy_stale = copy_stale

    def __call__(self, command, *, cwd, env, check):
        profile = command[command.index("--profile") + 1] if "--profile" in command else None
        is_pdf = "--to" in command and command[command.index("--to") + 1] == "pdf"
        if is_pdf:
            assert command[command.index("-M") + 1] == "latex-auto-install:false"
        label = profile or ("student" if is_pdf else "website")
        output = Path(command[command.index("--output-dir") + 1])
        source = Path(command[2])
        self.calls.append((source, label, output, cwd, env.copy(), check))
        if label == self.fail:
            raise subprocess.CalledProcessError(23, command)
        if label == self.omit:
            return
        destination = output / source.with_suffix(".pdf" if is_pdf else ".html")
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(("%PDF-" + label if is_pdf else "<html>" + label).encode())
        if not is_pdf:
            resource = output / "site_libs" / "resource.js"
            resource.parent.mkdir(parents=True, exist_ok=True)
            resource.write_bytes(b"resource")
            if self.copy_stale:
                for relative in ("labs/lab_01_beispiel.pdf", "labs/lab_01_beispiel-musterloesung.pdf", "vorlesungen/01_kickoff-handout.pdf", "labs/lab_01_beispiel-startcode.zip", "code/labs/lab_01_beispiel/solution/answer.py"):
                    stale = output / relative
                    stale.parent.mkdir(parents=True, exist_ok=True)
                    stale.write_bytes(b"stale source-side PDF copied by Quarto")


class BuildMaterialsTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="materials-test-")
        self.root = Path(self.temporary.name)
        self.messages = []

    def tearDown(self):
        self.temporary.cleanup()

    def write(self, relative, content="source"):
        path = self.root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding="utf-8")
        return path

    def fixture(self):
        self.write("index.qmd")
        self.write("vorlesungen/01_kickoff.qmd")
        self.write("labs/lab_01_beispiel.qmd")

    def builder(self, runner=None):
        return materials.MaterialBuilder(self.root, runner=runner or FakeQuarto(), report=self.messages.append)

    def test_recursive_selection_excludes_templates_includes_and_non_numbered_labs(self):
        self.fixture()
        self.write("vorlesungen/Vertiefung/02_überblick mit Leerzeichen.qmd")
        self.write("labs/Vertiefung/lab_02_beispiel.qmd")
        self.write("labs/Prüfung/probeklausur.qmd")
        for relative in (
            "template.qmd", "_include.qmd", "vorlesungen/folien_template.qmd",
            "vorlesungen/_include.qmd", "vorlesungen/_cache/03_alt.qmd",
            "labs/lab_template.qmd", "labs/lab_03_template.qmd", "labs/notizen.qmd",
            "labs/lab_1_falsch.qmd", "labs/lab_001_falsch.qmd", "labs/template/lab_04_beispiel.qmd",
        ):
            self.write(relative)
        sources = materials.discover_sources(self.root)
        relative = lambda paths: {str(path.relative_to(self.root)) for path in paths}
        self.assertEqual(relative(sources["website"]), {
            "index.qmd", "vorlesungen/01_kickoff.qmd", "vorlesungen/Vertiefung/02_überblick mit Leerzeichen.qmd",
        })
        self.assertEqual(relative(sources["labs"]), {
            "labs/lab_01_beispiel.qmd", "labs/Vertiefung/lab_02_beispiel.qmd", "labs/Prüfung/probeklausur.qmd",
        })

    def test_all_builds_publish_expected_names_and_compatibility_copies(self):
        self.fixture()
        self.write("labs/nested/probeklausur.qmd")
        before = {path: path.read_bytes() for path in self.root.rglob("*.qmd")}
        runner = FakeQuarto(copy_stale=True)
        self.builder(runner).build("all")
        expected = {
            "_site/index.html": b"<html>website",
            "_site/site_libs/resource.js": b"resource",
            "_site/vorlesungen/01_kickoff-handout.pdf": b"%PDF-handout",
            "_handout/vorlesungen/01_kickoff.pdf": b"%PDF-handout",
            "_site/labs/lab_01_beispiel.pdf": b"%PDF-student",
            "_site/labs/lab_01_beispiel-musterloesung.pdf": b"%PDF-solution",
            "labs/_output/lab_01_beispiel-studierende.pdf": b"%PDF-student",
            "labs/_output/lab_01_beispiel-musterloesung.pdf": b"%PDF-solution",
            "_site/labs/nested/probeklausur.pdf": b"%PDF-student",
            "_site/labs/nested/probeklausur-musterloesung.pdf": b"%PDF-solution",
        }
        for relative, content in expected.items():
            with self.subTest(relative=relative):
                self.assertEqual((self.root / relative).read_bytes(), content)
        self.assertEqual(before, {path: path.read_bytes() for path in self.root.rglob("*.qmd")})
        self.assertFalse((self.root / "labs/lab_01_beispiel.pdf").exists())
        self.assertFalse(list((self.root / ".quarto/materials-build").glob("run-*")))

    def test_student_and_solution_profiles_use_separate_fresh_output_directories(self):
        self.fixture()
        runner = FakeQuarto()
        with patch.dict(os.environ, {"QUARTO_PROFILE": "solution"}):
            self.builder(runner).build("labs")
        self.assertEqual([call[1] for call in runner.calls], ["student", "solution"])
        self.assertNotEqual(runner.calls[0][2], runner.calls[1][2])
        for source, label, output, cwd, env, check in runner.calls:
            self.assertEqual(cwd, self.root / "labs")
            self.assertNotIn("QUARTO_PROFILE", env)
            self.assertTrue(check)

    def test_required_late_failure_does_not_publish_a_partial_build(self):
        self.fixture()
        previous = self.write("_site/index.html", "previous website")
        stale = self.write("labs/lab_01_beispiel.pdf", "stale source output")
        runner = FakeQuarto(fail="solution")
        with self.assertRaisesRegex(materials.BuildError, "solution"):
            self.builder(runner).build("all")
        self.assertEqual(previous.read_text(), "previous website")
        self.assertEqual(stale.read_text(), "stale source output")
        self.assertFalse((self.root / "_site/labs/lab_01_beispiel.pdf").exists())
        self.assertFalse((self.root / ".quarto/materials-build/manifest.json").exists())

    def test_success_exit_without_fresh_expected_output_is_a_failure(self):
        self.fixture()
        self.write("labs/lab_01_beispiel.pdf", "stale PDF must never be reused")
        with self.assertRaisesRegex(materials.BuildError, "Ausgabe fehlt"):
            self.builder(FakeQuarto(omit="student")).build("labs")
        self.assertFalse((self.root / "_site").exists())

    def test_colliding_student_and_solution_filenames_fail_without_publishing(self):
        self.fixture()
        self.write("labs/lab_01_beispiel-musterloesung.qmd")
        with self.assertRaisesRegex(materials.BuildError, "denselben Ausgabepfad"):
            self.builder().build("labs")
        self.assertFalse((self.root / "_site").exists())

    def test_failed_rebuild_preserves_previous_outputs_and_manifest(self):
        self.fixture()
        builder = self.builder()
        builder.build("labs")
        previous = builder.manifest.read_bytes()
        with self.assertRaises(materials.BuildError):
            self.builder(FakeQuarto(fail="solution")).build("labs")
        self.assertEqual(builder.manifest.read_bytes(), previous)
        self.assertEqual((self.root / "_site/labs/lab_01_beispiel.pdf").read_bytes(), b"%PDF-student")

    def test_website_never_publishes_old_materials_copied_as_resources(self):
        self.fixture()
        self.builder(FakeQuarto(copy_stale=True)).build("website")
        self.assertFalse((self.root / "_site/labs/lab_01_beispiel.pdf").exists())
        self.assertFalse((self.root / "_site/vorlesungen/01_kickoff-handout.pdf").exists())
        self.assertFalse((self.root / "_site/labs/lab_01_beispiel-startcode.zip").exists())
        self.assertFalse((self.root / "_site/code/labs/lab_01_beispiel/solution/answer.py").exists())
        self.builder().build("labs")
        self.builder(FakeQuarto(copy_stale=True)).build("website")
        self.assertEqual((self.root / "_site/labs/lab_01_beispiel.pdf").read_bytes(), b"%PDF-student")
        self.assertEqual((self.root / "_site/labs/lab_01_beispiel-musterloesung.pdf").read_bytes(), b"%PDF-solution")

    def test_labs_build_includes_verified_packages_and_code_only_needs_no_quarto(self):
        self.fixture()

        def packages(root, stage, report):
            stage.mkdir(parents=True)
            artifact = stage / "verified.zip"
            artifact.write_bytes(b"fresh verified student archive")
            return {"_site/labs/lab_01_beispiel-startcode.zip": artifact}

        with patch.object(materials, "build_programming_packages", side_effect=packages) as package_build:
            self.builder().build("labs")
            self.assertEqual(package_build.call_count, 1)
            self.assertEqual((self.root / "_site/labs/lab_01_beispiel-startcode.zip").read_bytes(), b"fresh verified student archive")
            runner = FakeQuarto()
            self.builder(runner).build("code")
            self.assertFalse(runner.calls)

    def test_code_failure_prevents_publishing_successful_document_builds(self):
        self.fixture()
        previous = self.write("_site/index.html", "previous successful build")
        with patch.object(materials, "build_programming_packages", side_effect=materials.ProgrammingLabError("unexpected starter error")):
            with self.assertRaisesRegex(materials.BuildError, "unexpected starter error"):
                self.builder().build("all")
        self.assertEqual(previous.read_text(), "previous successful build")
        self.assertFalse((self.root / "_site/labs/lab_01_beispiel.pdf").exists())
        self.assertFalse((self.root / ".quarto/materials-build/manifest.json").exists())

    def test_removed_programming_packages_clean_only_owned_zip_outputs(self):
        self.fixture()

        def packages(root, stage, report):
            stage.mkdir(parents=True)
            artifact = stage / "verified.zip"
            artifact.write_bytes(b"archive")
            return {"_site/labs/lab_01_beispiel-startcode.zip": artifact}

        with patch.object(materials, "build_programming_packages", side_effect=packages):
            self.builder().build("code")
        untouched = self.write("_site/labs/custom.zip", "manually supplied archive")
        self.builder().build("code")
        self.assertFalse((self.root / "_site/labs/lab_01_beispiel-startcode.zip").exists())
        self.assertEqual(untouched.read_text(), "manually supplied archive")

    def test_removed_sources_clean_only_previously_owned_outputs(self):
        self.fixture()
        self.builder().build("all")
        untouched = self.write("_site/labs/custom.pdf", "user artifact")
        raw = self.write("labs/lab_01_beispiel.pdf", "manual source-side artifact")
        (self.root / "labs/lab_01_beispiel.qmd").unlink()
        self.builder().build("labs")
        self.assertFalse((self.root / "_site/labs/lab_01_beispiel.pdf").exists())
        self.assertFalse((self.root / "_site/labs/lab_01_beispiel-musterloesung.pdf").exists())
        self.assertFalse((self.root / "labs/_output/lab_01_beispiel-studierende.pdf").exists())
        self.assertEqual(untouched.read_text(), "user artifact")
        self.assertEqual(raw.read_text(), "manual source-side artifact")
        self.assertTrue((self.root / "_site/index.html").exists())
        self.assertTrue(any("keine passenden Quellen" in message for message in self.messages))

    def test_empty_website_fails_and_empty_optional_categories_are_reported(self):
        with self.assertRaisesRegex(materials.BuildError, "keine .qmd-Quellen"):
            self.builder().build("website")
        self.builder().build("labs")
        self.builder().build("handouts")
        self.assertTrue(any(message == "labs: 0 Quelle(n)." for message in self.messages))
        self.assertTrue(any(message == "handouts: 0 Quelle(n)." for message in self.messages))

    def test_invalid_manifest_cannot_delete_unowned_paths(self):
        self.fixture()
        manifest = self.write(".quarto/materials-build/manifest.json", '{"labs": ["../outside.pdf"]}')
        runner = FakeQuarto()
        with self.assertRaisesRegex(materials.BuildError, "Manifest"):
            self.builder(runner).build("labs")
        self.assertFalse(runner.calls)
        self.assertEqual(manifest.read_text(), '{"labs": ["../outside.pdf"]}')

    def test_cli_returns_failure_status(self):
        with patch.object(materials.MaterialBuilder, "build", side_effect=materials.BuildError("test failure")):
            with contextlib.redirect_stderr(io.StringIO()) as error:
                self.assertEqual(materials.main(["all"]), 1)
        self.assertIn("Build fehlgeschlagen: test failure", error.getvalue())


if __name__ == "__main__":
    unittest.main()
