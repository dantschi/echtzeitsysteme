"""Referenzlösung zu Übungsblatt 2; Python 3.12, nur Standardbibliothek."""


def first_index(values: list[int], target: int) -> int:
    """Ersten nullbasierten Index von target liefern, sonst -1."""
    for index, value in enumerate(values):
        if value == target:
            return index
    return -1


def count_matches(values: list[int], target: int) -> int:
    """Anzahl aller Vorkommen von target liefern, sonst 0."""
    count = 0
    for value in values:
        if value == target:
            count += 1
    return count
