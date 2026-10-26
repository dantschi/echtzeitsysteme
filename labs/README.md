# Übungsblätter: Papierklausur und eigene Implementierungen

Alle neun Blätter beginnen unmittelbar mit den Aufgaben. Die Klausurvorbereitung umfasst neue, vollständig angegebene Papierbeispiele mit Rechenwegen und Begründungen; die Beispiele werden nicht unverändert aus den Folien übernommen. Jedes Blatt bietet außerdem eine kleine eigene Implementierungsaufgabe auf einer vorbereiteten Projektbasis. Die Studierenden schreiben die jeweilige Anwendungslogik selbst.

Die Modulnote entsteht durch die Papierklausur. Jedes Blatt enthält 32 Orientierungspunkte sowie zusätzliche Verständnis- und Programmieraufgaben ohne weitere Punkte. Blatt 5 hat nach der Überarbeitung ebenfalls 32 Punkte. Lösungen stehen ausschließlich im Quarto-Profil `solution`. Der vollständige Aufgabenpool darf länger als der verfügbare Termin sein; die Lehrperson wählt Aufgaben aus. Die Zeitangaben sind Richtwerte für die Programmierung auf einer bereits funktionierenden Basis, keine Obergrenze des gesamten Blatts.

| Blatt | Papieraufgaben | Eigene Implementierung | Praxis, ca. min |
|---|---|---|---:|
| [1](lab_01_bare-metal-polling-interrupts.qmd) | Zeitvertrag, Antwort, Jitter, Polling, Timer | Taster-Nachleuchtanzeige ohne blockierendes Warten | 25–35 |
| [2](lab_02_scheduling-theorie-kooperativer-scheduler.qmd) | Jobmodell, RMS, Antwortzeitanalyse, EDF, kooperative Runden | Doppel-/Dreifachpuls als kooperativer Automat | 25–35 |
| [3](lab_03_einfuehrung-freertos-praeemption.qmd) | Threadzustände, Präemption, Ticks, Drift, Speicher, Flags | zwei FreeRTOS-Threads mit LEDs: CubeMX-Konfiguration und Code direkt im Blatt | 30–40 |
| [4](lab_04_race-conditions-mutex-priority-inversion.qmd) | Lost Update, Schutzumfang, Semaphor, Deadlock, Inversion | Mutex-Änderung und konsistenter Snapshot | 30–40 |
| [5](lab_05_deferred-interrupt-processing.qmd) | FIFO-Zustände, Überlauf, Besitz, IRQ-Übergabe | Taster-IRQ mit Wertqueue und LED-Worker | 35–45 |
| [6](lab_06_multicore-shared-memory-cache-kohaerenz.qmd) | AMP, Shared-Layout, Phasenbesitz, Cache, MPU | eigene geordnete M4-Antwort auf neue M7-Anfragen | 25–35 |
| [7](lab_07_tracing-timing-analyse.qmd) | Messvertrag, Antwortzeit, Modulo, Stichprobe, Loglücken | eigene Gruppenauswertung mit 64-Bit-Summe | 30–40 |
| [8](lab_08_server-verfahren.qmd) | CPU-Budget, Polling-RTA, Deferrable/Sporadic, Überlast | eigenes Polling-Modell mit zwei CM4-Threads und LEDs | 25–35 |
| [9](lab_09_capstone-timing-verifikation.qmd) | Taskmodell, Polling, Queue, Hysterese, Zeitbefund, Fehlerreaktion | begrenzte Verarbeitung mit Hysterese- und Eingabesequenzanzeige | 30–40 |

## Board und Vorbereitung

Verwendet werden **NUCLEO-H755ZI-Q / STM32H755ZIT6**, **STM32CubeMX** und **VS Code mit STM32 VS Code Extension**. Board und USB-Verbindung zum integrierten ST-LINK genügen. Die Aufgaben verwenden ausschließlich die eingebauten LEDs **LD1/PB0**, **LD2/PE1**, **LD3/PB14** und gegebenenfalls den Benutzertaster **B1/PC13**. Pinbelegung, aktive Polarität und Boardrevision gegen UM2408 prüfen. Zähler und Resultate sind mit dem vorhandenen Debugger beobachtbar; zusätzliche Elektronik, UART-Verdrahtung, Mess- oder Tracegeräte werden nicht vorausgesetzt.

Installation, zur Aufgabe passender Kernstart, gültige GPIO-/Kernelkonfiguration sowie bei Bedarf Linker-, Shared- und MPU-Einrichtung werden vor dem Termin vorbereitet. Die jeweilige [Projektanleitung](vertiefung/) benennt Quellorte, Integrationspunkte und den erwarteten Ausgangsstand. Blatt 3 verwendet direkt die von CubeMX erzeugten Threadfunktionen; seine Codebausteine stehen im Aufgabenblatt. Eine erneute CubeMX-Generierung mit anschließendem Build muss die Einbindung erhalten. Anwendungsbausteine im Repository ersetzen kein generiertes Firmwaregesamtprojekt und keinen tatsächlichen Boardtest.

CM4 führt in Blatt 1/2 Bare-Metal- und in Blatt 4/5 RTOS-Anwendungen aus; CM7 behält dort jeweils den generierten Systemstart. **Blatt 3, 8 und 9 nutzen ausschließlich CM4, einschließlich Systemstart und Taktkonfiguration; CM7 bleibt boot-deaktiviert.** Blatt 6 nutzt beide Kerne mit vorbereitetem Kanal. Blatt 7 misst und analysiert lokal auf CM7 mit vorbereitetem Grundstart. GPIO-Konfiguration und jeder LED-Ausgang haben genau einen Besitzer; verschiedene Pins desselben Ports sind kein Freibrief für konkurrierende Registeränderungen.

Die Papieraufgaben setzen keinen erfolgreichen Firmwarelauf voraus. Im Praxisteil werden eigene Funktionen implementiert, geprüft und erklärt. Technische Fehlersuche wird vom Umfang der ausgewählten Aufgaben getrennt geplant; eine reine Vorführung ersetzt die Implementierungsaufgabe nicht. Die alten vollständigen Demonstratoren sind, soweit vorhanden, als getrennte Zusatzversuche dokumentiert und werden nicht gleichzeitig mit den neuen Anwendungsvarianten gebaut.

## Prüfen und dokumentieren

Zeitachsen beschriften, Einheiten nennen, Modellannahmen und reale Beobachtungen unterscheiden. Diagnosefelder nur am angegebenen konsistenten Prüfpunkt lesen, bei Shared-Daten gegebenenfalls beide Kerne anhalten. Ein Debughalt ist keine normale Timingbedingung. Ein Messmaximum ist keine WCET; eine gültig beobachtete Fristverletzung bleibt hingegen ein Gegenbeispiel. Ohne Hardwarelauf oder Zeitmessung deren Status ausdrücklich offenlassen.

Die LED-Simulation in Blatt 8 verwendet virtuelle Modellzeit und reserviert keine echte CPU-Zeit im FreeRTOS-Kernel. Blatt 9 verbindet die Serveranalyse mit Datenverträgen, Frischeprüfung und Watchdog-Grundideen auf Papier. Der kleine Praxisversuch begrenzt eine Nachrichtenanzahl und zeigt Hysterese sowie Eingabesequenzlücken; daraus folgt kein nachgewiesenes CPU-Budget. Ein LED-Zustand ist kein sicherheitsgerichteter Freigabeausgang.

## Musterlösungen und PDF-Build

Alle Musterlösungen verwenden `::: {.content-visible when-profile="solution"}`. Soweit studentische Startdateien bereitgestellt werden, enthalten sie bewusst noch keine Aufgabenlösung. In Blatt 8 und 9 stehen die nötigen Codebausteine direkt im Blatt und werden in die von CubeMX erzeugten Funktionen übernommen. Beispiel aus `labs/`:

```bash
quarto render lab_01_bare-metal-polling-interrupts.qmd --to pdf --profile solution
quarto render lab_01_bare-metal-polling-interrupts.qmd --to pdf
```

`build_labs.sh` erzeugt beide Fassungen unter `labs/_output/`; die kanonischen PDFs unter `labs/` bleiben studentische Fassungen. Projektanleitungen sind offen lesbare Dokumentation, keine zusätzlichen Pflichtblätter. Die PDFs verlinken sie auf GitHub, weil die Kurswebsite nicht automatisch die Markdown-Anleitungen veröffentlicht. Beide Fassungen verwenden helle Codeblöcke für den Druck.

Die [Probeklausur](probeklausur.qmd) bleibt bei etwa 60 Minuten und 60 Punkten: 20 Multiple-Choice-Fragen mit je vier Optionen, danach vier Papieraufgaben. Die Fragen zu den Terminen 7–9 und die integrierte Papieraufgabe sind auf die überarbeiteten Vorlesungen, Übungen und Quizze abgestimmt.
