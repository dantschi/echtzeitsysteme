"""Öffentliche Tests; aus starter/ oder solution/ mit Python 3.12 aufrufen."""

import importlib
import json
from pathlib import Path
import sys
from types import ModuleType
from typing import Callable


def expect_equal(actual: int, expected: int) -> None:
    if actual != expected:
        raise AssertionError(f"Erwartet: {expected}; erhalten: {actual!r}")


def test_first(search: ModuleType) -> None:
    values = [4, 7, 7, 2]
    expect_equal(search.first_index(values, 7), 1)
    expect_equal(search.first_index([7, 2, 7], 7), 0)
    if values != [4, 7, 7, 2]:
        raise AssertionError("Die Eingabeliste wurde verändert.")


def test_notfound(search: ModuleType) -> None:
    expect_equal(search.first_index([-3, 0, 9], 4), -1)


def test_count(search: ModuleType) -> None:
    values = [4, 7, 7, 2]
    expect_equal(search.count_matches(values, 7), 2)
    expect_equal(search.count_matches(values, 9), 0)
    expect_equal(search.count_matches([7, 7, 7], 7), 3)
    if values != [4, 7, 7, 2]:
        raise AssertionError("Die Eingabeliste wurde verändert.")


def test_empty(search: ModuleType) -> None:
    expect_equal(search.first_index([], 7), -1)
    expect_equal(search.count_matches([], 7), 0)


TESTS: list[tuple[str, Callable[[ModuleType], None]]] = [
    ("test_first", test_first),
    ("test_notfound", test_notfound),
    ("test_count", test_count),
    ("test_empty", test_empty),
]


def main() -> int:
    failures: list[str] = []
    errors: list[str] = []
    sys.dont_write_bytecode = True
    sys.path.insert(0, str(Path.cwd()))
    try:
        search = importlib.import_module("search")
    except Exception as error:
        errors.append(f"import: {type(error).__name__}: {error}")
    else:
        for test_id, test in TESTS:
            try:
                test(search)
            except (AssertionError, NotImplementedError):
                failures.append(test_id)
            except Exception as error:
                errors.append(f"{test_id}: {type(error).__name__}: {error}")

    print(json.dumps({"tests": len(TESTS), "failures": failures, "errors": errors}))
    return 1 if failures or errors else 0


if __name__ == "__main__":
    sys.exit(main())
