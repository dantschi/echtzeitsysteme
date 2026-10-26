# Projektvorbereitung zu Übungsblatt 7

Die Aufgabe ergänzt das vorhandene vorbereitete Projekt für **NUCLEO-H755ZI-Q / STM32H755ZIT6**. Verwendet werden STM32CubeMX sowie VS Code mit STM32 VS Code Extension. Installation, erstmaliger Build, Flashen und Bootprüfung erfolgen vor dem Termin. Die Studierenden implementieren im Termin die kleine Auswertungsfunktion selbst; die vorbereitete Zählerroutine bleibt erhalten.

Board, USB/ST-LINK und die eingebaute LD1 genügen. B1 wird für diese Aufgabe nicht benötigt. Es sind keine zusätzliche Elektronik, UART-Verkabelung oder externen Mess- und Tracegeräte erforderlich. Alle Papieraufgaben bleiben ohne Hardware vollständig lösbar.

## Vorbereiteter Projektstand

- Die installierte Werkzeugkombination unterstützt die gewählten Cortex-M7-/Cortex-M4-Ziele. Der vorhandene Dual-Core-Boot einschließlich generierter HSEM-Startfolge bleibt erhalten; beide Images passen zusammen. Der M4 erhält nach dem Boot keine zusätzliche Laborlast.
- Takt, Startup, Linker sowie nötige Speicher-/Cachekonfiguration stammen aus der geprüften Projektbasis. Der tatsächliche M7-Kerntakt stimmt mit dem aktualisierten `SystemCoreClock` überein. Die Messung verlangt privilegierte M7-Ausführung.
- Eigene Quellen und Benutzerbereiche überstehen eine erneute CubeMX-Generierung und den anschließenden Build. Es gibt keine zweite Instanz der Laborroutine, keinen zusätzlichen Callback und keinen konkurrierenden Schreiber des Logs. C11 oder neuer wird verwendet.
- DWT bedeutet Data Watchpoint and Trace. Verfügbarkeit und Lauf seines lokalen Zykluszählers `CYCCNT`, dessen Verhalten bei Debughalt beziehungsweise Sleep und die Zuständigkeit für diesen Zähler werden am tatsächlichen Projekt geprüft. Kein zweiter Debug-/Traceclient setzt ihn während des Versuchs zurück. Der Versuch benötigt keine Einrichtung von ITM, ETM oder externer Traceaufnahme.

## Einbindung auf dem Cortex-M7

1. `code/lab07/lab07_timing.c` und `lab07_timing.h` in das M7-Ziel aufnehmen. Für diese Aufgabe die Compile-Definitionen `LAB07_SAMPLES_PER_KIND=5`, `LAB07_WORK_STEPS=15000` und `LAB07_WORK_SEED=13` setzen. Damit sind fünf EMPTY- und fünf WORK-Datensätze vorgesehen. Die unveränderte Quelldatei verwendet ohne Definitionen weiterhin ihre bisherigen Standardwerte. Beide Gruppen müssen in den festen Log passen; das prüft eine statische Zusicherung beim Übersetzen.
2. `lab07_summary.c`, `summary_student.c` und den zugehörigen Headerpfad zusätzlich ins M7-Ziel aufnehmen. In CubeMX LD1 auf PB0 mit User Label `LAB07_RESULT` als Ausgang mit initialem Pegel LOW ausschließlich dem M7 zuordnen. Der M4 konfiguriert diesen Port nicht.
3. Nach bestätigter Takt- und GPIO-Initialisierung `lab07_timing.h` und `lab07_summary.h` einbinden. Genau einmal `Lab07_Init(SystemCoreClock)` und danach `Lab07_Run()` auf `LAB07_OK` prüfen. Nur nach beiden Erfolgen einmal `Lab07_ShowAnalysis()` aufrufen. Bei Fehler wird der bestehende Fehlerpfad der Projektbasis verwendet. Die Funktionen werden weder in einem Interruptbehandler noch wiederholt aus der Hauptschleife aufgerufen.
4. Vor der Ausgabe des Basisstands den vorgesehenen Ablauf am Board prüfen: Nach erfolgreichem Lauf sind `lab07_done == 1` und zehn Datensätze mit fünf Einträgen je Art zu erwarten. Mit der unveränderten studentischen Startfunktion bleibt `lab07_analysis_valid` null und LD1 aus. Das ist die erwartete Ausgangslage vor Implementierung und kein Defekt des Messlaufs.
5. Die Studierenden implementieren `Lab07_Summarize()` in `summary_student.c`. Nach erfolgreicher Implementierung wird WORK durch `Lab07_ShowAnalysis()` ausgewertet. Für die separate EMPTY-Auswertung kann nach beendetem Log `Lab07_Summarize()` mit dieser Auswahl und einer eigenen Ergebnisstruktur aufgerufen werden. Der Log wird dafür nicht erneut beschrieben.

## Messvertrag und Auswertung

Das Fenster reicht von der ersten bis zur zweiten lokalen Zählerlesung. Die vorgegebene Routine speichert den Datensatz erst nach der Endlesung. EMPTY enthält den Messpfad ohne synthetische Arbeitsfunktion, WORK zusätzlich diese Arbeitsfunktion. Die Messfenster enthalten dennoch ihren eigenen Befehlsaufwand und gegebenenfalls dazwischenliegende Interruptarbeit. Der resultierende Wert ist ein Intervall in lokalen Kerntakten, keine automatisch reine eigene CPU-Zeit.

Die zeitliche Umrechnung setzt stabilen tatsächlichen Kerntakt, durchlaufenden Zähler und ein Fenster kürzer als einen vollständigen Umlauf voraus. Diese Voraussetzungen sind durch die Lehrperson für die Basis zu prüfen. Ein unverändertes `SystemCoreClock` allein beweist keinen unveränderten tatsächlichen Takt. Die Statusprüfungen der Basis erkennen einige Konfigurationsfehler; sie beweisen weder kontinuierlichen Zählerbetrieb noch die Abwesenheit versteckter vollständiger Umläufe. Die nachgeschaltete Gruppenauswertung kann solche Fehler nicht reparieren.

Logs werden erst nach ihrem Ende und bei angehaltenem zuständigem Kern über die zum Image und Cache passende Debuggersicht gelesen. Keine Ausgabe und kein manueller Debughalt gehören ins laufende Messfenster. Die Studierenden dokumentieren Anzahl, Minimum, Maximum, 64-Bit-Summe und tatsächlichen Kerntakt je Gruppe. EMPTY und WORK werden getrennt beschrieben; ein pauschaler Leerfensterabzug und ein endliches Maximum ersetzen keinen WCET- oder Antwortfristnachweis.

## Prüfstatus festhalten

Vor dem Termin wird ein auf der tatsächlichen Hardware gebauter und gestarteter Basisstand bereitgestellt. Die studentische Startdatei enthält bewusst keine Aufgabenlösung. Vollständige Musterfunktionen und begründete Rechenergebnisse stehen ausschließlich im zugehörigen Übungsblatt unter Quarto-Profil `solution`.

Quelltextprüfung, Hosttests einer reinen C-Auswertung und ein erfolgreicher Dokumentbuild sind jeweils eigene Nachweise. Sie bestätigen keinen STM32-Build, Boardlauf, tatsächlichen Takt oder reale Zeitgrenzen. Ein Protokoll nennt deshalb Werkzeugversionen, Build-/Boardstatus, Messvertrag und tatsächlich erhobene Werte. Ungemessene Zeiten bleiben offen.
