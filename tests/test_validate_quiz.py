"""Behavioral tests for the read-only QuizAcademy format validator."""

import contextlib
import csv
import importlib.util
import io
from pathlib import Path
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / ".agents/skills/quiz-erstellen/scripts/validate_quiz.py"
SPEC = importlib.util.spec_from_file_location("validate_quiz", SCRIPT)
quiz = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(quiz)


def valid_question():
    return [
        'Welche Aussage zu „Überlauf" ist richtig?', "Multiple Choice",
        "Eine richtige Aussage", "correct", "Verwechslung A", "incorrect",
        "Verwechslung B", "incorrect", "Verwechslung C", "incorrect",
        'Die Antwort erläutert das Beispiel mit "Anführungszeichen".',
    ]


class ValidateQuizTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.directory = Path(self.temporary.name)

    def write(self, rows=None, name="quiz.csv", encoding="utf-8"):
        path = self.directory / name
        with path.open("w", encoding=encoding, newline="") as handle:
            csv.writer(handle, delimiter=";").writerows(
                [quiz.HEADER, valid_question()] if rows is None else rows
            )
        return path

    def test_valid_utf8_quotes_and_input_is_unchanged(self):
        path = self.write()
        before = path.read_bytes()
        self.assertEqual(quiz.validate_file(path), (1, []))
        self.assertEqual(path.read_bytes(), before)

    def test_utf8_bom_and_quoted_multiline_are_supported(self):
        row = valid_question()
        row[0] = "Eine Frage\nmit Kontext in einer zweiten Zeile?"
        path = self.write([quiz.HEADER, row], encoding="utf-8-sig")
        self.assertEqual(quiz.validate_file(path), (1, []))

    def test_exact_header_is_required(self):
        header = list(quiz.HEADER)
        header[10] = "Erklärung"
        _, errors = quiz.validate_file(self.write([header, valid_question()]))
        self.assertTrue(any("Kopfzeile" in e and "Datensatz 1, Zeile 1" in e for e in errors))

    def test_wrong_column_count_and_blank_record_fail(self):
        for row in [valid_question()[:-1], valid_question() + ["Zusatz"], []]:
            with self.subTest(row=row):
                _, errors = quiz.validate_file(self.write([quiz.HEADER, row]))
                self.assertTrue(any("elf Spalten" in e for e in errors))

    def test_nonempty_question_answers_and_explanation_are_required(self):
        for index in [0, 2, 4, 6, 8, 10]:
            with self.subTest(index=index):
                row = valid_question()
                row[index] = " \t"
                _, errors = quiz.validate_file(self.write([quiz.HEADER, row]))
                self.assertTrue(errors)

    def test_duplicate_options_fail_without_collapsing_case(self):
        row = valid_question()
        row[4] = " Eine richtige Aussage "
        _, errors = quiz.validate_file(self.write([quiz.HEADER, row]))
        self.assertTrue(any("unterschiedlich" in e for e in errors))
        row[2], row[4] = "variable", "Variable"
        self.assertEqual(quiz.validate_file(self.write([quiz.HEADER, row])), (1, []))

    def test_type_and_correctness_flags_are_exact(self):
        for index, value in [(1, "Single Choice"), (3, "true"), (5, "correct"), (3, "incorrect")]:
            with self.subTest(index=index, value=value):
                row = valid_question()
                row[index] = value
                self.assertTrue(quiz.validate_file(self.write([quiz.HEADER, row]))[1])

    def test_semicolons_inside_quoted_fields_are_rejected(self):
        for index in [0, 2, 10]:
            with self.subTest(index=index):
                row = valid_question()
                row[index] = "Text; trotz korrektem CSV-Quoting unzulässig"
                _, errors = quiz.validate_file(self.write([quiz.HEADER, row]))
                self.assertTrue(any("Semikolon" in e for e in errors))

    def test_malformed_quoting_fails_with_location(self):
        path = self.directory / "broken.csv"
        path.write_text(";".join(quiz.HEADER) + '\n"nicht abgeschlossen', encoding="utf-8")
        _, errors = quiz.validate_file(path)
        self.assertTrue(any("Ungültiges CSV" in e and "Datensatz 2, Zeile 2" in e for e in errors))

    def test_record_start_line_tracks_quoted_multiline(self):
        row = valid_question()
        row[0] = "Kontext\nFortsetzung"
        invalid = valid_question()
        invalid[1] = "wrong"
        _, errors = quiz.validate_file(self.write([quiz.HEADER, row, invalid]))
        self.assertTrue(any("Datensatz 3, Zeile 4" in e for e in errors))

    def test_empty_and_header_only_files_do_not_validate_a_quiz(self):
        for rows in [[], [quiz.HEADER]]:
            with self.subTest(rows=rows):
                count, errors = quiz.validate_file(self.write(rows))
                self.assertEqual(count, 0)
                self.assertTrue(errors)

    def test_unreadable_missing_and_non_utf8_inputs_fail(self):
        invalid = self.directory / "invalid.csv"
        invalid.write_bytes(b"\xff\xfe")
        for path in [self.directory / "missing.csv", self.directory, invalid]:
            with self.subTest(path=path):
                _, errors = quiz.validate_file(path)
                self.assertTrue(any(str(path) in e and "gelesen" in e for e in errors))

    def test_cli_processes_multiple_files_and_returns_nonzero_on_any_failure(self):
        good = self.write(name="good.csv")
        bad = self.write([quiz.HEADER], name="bad.csv")
        stdout, stderr = io.StringIO(), io.StringIO()
        with contextlib.redirect_stdout(stdout), contextlib.redirect_stderr(stderr):
            result = quiz.main([str(bad), str(good)])
        self.assertEqual(result, 1)
        self.assertIn(str(good), stdout.getvalue())
        self.assertIn(str(bad), stderr.getvalue())

    def test_cli_valid_file_returns_zero(self):
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(quiz.main([str(self.write())]), 0)


if __name__ == "__main__":
    unittest.main()
