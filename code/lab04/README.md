# Implementierungsaufgabe zu Übungsblatt 4

`lab04_account.c/.h` und `account_student.c`: eigene unter Mutex geschützte Änderung und konsistenter Snapshot, drei Board-LEDs. `lab04_resources.c/.h` bleibt ein getrennter Inversionsdemonstrator.

[Projektvorbereitung](../../labs/vertiefung/lab_04_programmierung.md) · [Aufgaben und Musterlösung](../../labs/lab_04_race-conditions-mutex-priority-inversion.qmd)

Die studentischen Startdateien enthalten bewusst noch keine Lösung. Vollständige Musterfunktionen stehen im Quarto-Profil `solution`. Benötigt werden NUCLEO-H755ZI-Q, STM32CubeMX und VS Code mit STM32 VS Code Extension; zusätzliche Elektronik oder Messgeräte sind nicht erforderlich. Die Dateien ergänzen ein vorbereitetes generiertes Projekt, sie ersetzen weder dessen Startup-/Linker-/Kernelkonfiguration noch einen tatsächlichen Boardtest.
