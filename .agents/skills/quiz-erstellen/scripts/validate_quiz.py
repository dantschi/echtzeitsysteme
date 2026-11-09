#!/usr/bin/env python3
"""Prüfe QuizAcademy-CSV-Dateien auf das Format dieses Templates (nur Standardbibliothek)."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path
import sys


HEADER = (
    "Frage", "Typ", "Antwort 1", "Korrekt 1", "Antwort 2", "Korrekt 2",
    "Antwort 3", "Korrekt 3", "Antwort 4", "Korrekt 4", "Erläuterung",
)
ANSWER_COLUMNS = (2, 4, 6, 8)
CORRECT_COLUMNS = (3, 5, 7, 9)


def validate_file(path: Path) -> tuple[int, list[str]]:
    """Return question count and diagnostics without modifying the source file.

    Record numbers include the header. Line numbers identify the first physical
    line of a record; quoted multiline fields are valid CSV and remain supported.
    This checks serialization and explicit structural rules, not factual accuracy.
    """
    path = Path(path)
    errors: list[str] = []
    records = 0

    def report(record: int, line: int, message: str) -> None:
        errors.append(f"{path}: Datensatz {record}, Zeile {line}: {message}")

    try:
        with path.open(encoding="utf-8-sig", newline="") as handle:
            reader = csv.reader(handle, delimiter=";", strict=True)
            while True:
                line = reader.line_num + 1
                try:
                    row = next(reader)
                except StopIteration:
                    break
                except csv.Error as error:
                    report(records + 1, line, f"Ungültiges CSV: {error}")
                    break
                records += 1
                if records == 1:
                    if tuple(row) != HEADER:
                        report(records, line, "Kopfzeile stimmt nicht mit dem vorgeschriebenen Elf-Spalten-Format überein.")
                    continue
                if len(row) != len(HEADER):
                    report(records, line, f"Erwartet sind elf Spalten, gefunden wurden {len(row)}.")
                    continue
                for index, value in enumerate(row):
                    if ";" in value:
                        report(records, line, f"Feld {HEADER[index]!r} enthält ein nicht erlaubtes Semikolon.")
                if not row[0].strip():
                    report(records, line, "Frage ist leer.")
                if row[1] != "Multiple Choice":
                    report(records, line, "Typ muss exakt 'Multiple Choice' sein.")
                answers = [row[index].strip() for index in ANSWER_COLUMNS]
                if not all(answers):
                    report(records, line, "Alle vier Antwortoptionen müssen nichtleer sein.")
                if len(set(answers)) != 4:
                    report(records, line, "Die vier Antwortoptionen müssen unterschiedlich sein.")
                flags = [row[index] for index in CORRECT_COLUMNS]
                if flags.count("correct") != 1 or flags.count("incorrect") != 3:
                    report(records, line, "Erwartet sind genau ein 'correct' und drei 'incorrect'.")
                if not row[10].strip():
                    report(records, line, "Erläuterung ist leer.")
    except (OSError, UnicodeError) as error:
        errors.append(f"{path}: Datei kann nicht als UTF-8-CSV gelesen werden: {error}")
        return max(0, records - 1), errors

    if records == 0:
        errors.append(f"{path}: CSV ist leer; Kopfzeile und Fragen fehlen.")
    elif records == 1:
        errors.append(f"{path}: Keine Fragen vorhanden; eine Kopfzeile allein validiert kein Quiz.")
    return max(0, records - 1), errors


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description=__doc__,
        epilog="Dateien werden nicht verändert. Eine erfolgreiche Formatprüfung ersetzt kein fachliches Review.",
    )
    parser.add_argument("files", type=Path, nargs="+", metavar="CSV", help="eine oder mehrere Quiz-CSV-Dateien")
    args = parser.parse_args(argv)
    failed = False
    for path in args.files:
        count, errors = validate_file(path)
        if errors:
            failed = True
            for error in errors:
                print(error, file=sys.stderr)
        else:
            print(f"{path}: Formatprüfung OK ({count} Frage(n)).")
    return int(failed)


if __name__ == "__main__":
    sys.exit(main())
