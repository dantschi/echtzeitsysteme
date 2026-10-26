# Implementierungsaufgabe zu Übungsblatt 8

Die vorhandene Cortex-M7-Routine erfasst kurze lokale Zählerfenster. Die Studierenden implementieren in `summary_student.c` die Gruppenauswertung `Lab08_Summarize()` mit Anzahl, Minimum, Maximum und 64-Bit-Summe sowie Argument- und Taktprüfung. `lab08_summary.c/.h` bindet sie nach beendetem Log ein; LD1 auf PB0 zeigt eine erfolgreiche WORK-Auswertung an.

Für die Übung wird `LAB08_SAMPLES_PER_KIND=5` im vorbereiteten M7-Ziel gesetzt: fünf EMPTY- und fünf WORK-Datensätze. Ohne diese Compile-Definition bleiben die bisherigen Quelltextstandardwerte erhalten. Details zu Integration und Messvertrag stehen in der [Projektvorbereitung](../../labs/vertiefung/lab_08_programmierung.md), Aufgaben und ausschließlich im Profil `solution` sichtbare Musterlösung im [Übungsblatt](../../labs/lab_08_tracing-timing-analyse.qmd).

Benötigt werden NUCLEO-H755ZI-Q, USB/ST-LINK, STM32CubeMX und VS Code mit STM32 VS Code Extension. Zusätzliche Elektronik oder ein externer Traceadapter sind nicht erforderlich. Die Dateien ergänzen eine vorbereitete Projektbasis und ersetzen weder deren Startup-/Linkerkonfiguration noch einen tatsächlichen Firmwarebuild und Boardtest. Die studentische Startfunktion gibt bis zur eigenen Implementierung absichtlich Fehler zurück. Es werden keine Hardwaremesswerte vorgegeben.
