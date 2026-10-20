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
| [7](lab_07_tracing-timing-analyse.qmd) | Messvertrag, Beobachtereffekt, Modulo, Tracequalität | robuste Gruppenauswertung mit 64-Bit-Summe | 30–40 |
| [8](lab_08_hsem-wwdg-safe-shutdown.qmd) | HSEM, Lost Wakeup, Restbudget, Watchdog, Ausgangsschutz | Frischeprüfung eines Supervisors mit Fehlerprovokation über B1 | 25–35 |
| [9](lab_09_capstone-timing-verifikation.qmd) | Architektur, Bilanzen, Zeitabnahme, Filter, Fortschritt | Hysterese- und Publikationslückenanzeige im Capstone | 30–40 |

## Board und Vorbereitung

Verwendet werden **NUCLEO-H755ZI-Q / STM32H755ZIT6**, **STM32CubeMX** und **VS Code mit STM32 VS Code Extension**. Board und USB-Verbindung zum integrierten ST-LINK genügen. Die Aufgaben verwenden ausschließlich die eingebauten LEDs **LD1/PB0**, **LD2/PE1**, **LD3/PB14** und gegebenenfalls den Benutzertaster **B1/PC13**. Pinbelegung, aktive Polarität und Boardrevision gegen UM2408 prüfen. Zähler und Resultate sind mit dem vorhandenen Debugger beobachtbar; zusätzliche Elektronik, UART-Verdrahtung, Mess- oder Tracegeräte werden nicht vorausgesetzt.

Installation, zur Aufgabe passender Kernstart, gültige GPIO-/Kernelkonfiguration sowie bei Bedarf Linker-, Shared- und MPU-Einrichtung werden vor dem Termin vorbereitet. Die jeweilige [Projektanleitung](vertiefung/) benennt Quellorte, Integrationspunkte und den erwarteten Ausgangsstand. Blatt 3 verwendet direkt die von CubeMX erzeugten Threadfunktionen; seine Codebausteine stehen im Aufgabenblatt. Eine erneute CubeMX-Generierung mit anschließendem Build muss die Einbindung erhalten. Anwendungsbausteine im Repository ersetzen kein generiertes Firmwaregesamtprojekt und keinen tatsächlichen Boardtest.

CM4 führt in Blatt 1/2 Bare-Metal- und in Blatt 4/5/8 RTOS-Anwendungen aus; CM7 behält dort jeweils den generierten Systemstart. **Blatt 3 nutzt ausschließlich CM4, einschließlich Systemstart und Taktkonfiguration; CM7 bleibt boot-deaktiviert.** Blatt 6/9 nutzt beide Kerne mit vorbereitetem Kanal. Blatt 7 misst und analysiert auf CM7. GPIO-Konfiguration und jeder LED-Ausgang haben genau einen Besitzer; verschiedene Pins desselben Ports sind kein Freibrief für konkurrierende Registeränderungen.

Die Papieraufgaben setzen keinen erfolgreichen Firmwarelauf voraus. Im Praxisteil werden eigene Funktionen implementiert, geprüft und erklärt. Technische Fehlersuche wird vom Umfang der ausgewählten Aufgaben getrennt geplant; eine reine Vorführung ersetzt die Implementierungsaufgabe nicht. Die alten vollständigen Demonstratoren sind, soweit vorhanden, als getrennte Zusatzversuche dokumentiert und werden nicht gleichzeitig mit den neuen Anwendungsvarianten gebaut.

## Prüfen und dokumentieren

Zeitachsen beschriften, Einheiten nennen, Modellannahmen und reale Beobachtungen unterscheiden. Diagnosefelder nur am angegebenen konsistenten Prüfpunkt lesen, bei Shared-Daten gegebenenfalls beide Kerne anhalten. Ein Debughalt ist keine normale Timingbedingung. Ein Messmaximum ist keine WCET; eine gültig beobachtete Fristverletzung bleibt hingegen ein Gegenbeispiel. Ohne Hardwarelauf oder Zeitmessung deren Status ausdrücklich offenlassen.

Die Fortschrittsaufgabe in Blatt 8 startet bewusst keinen Hardware-Watchdog: Sie prüft die Refresh-Voraussetzung anhand tatsächlicher Worker-Nachweise. Watchdogfenster und Reaktionsbudgets werden auf Papier berechnet. Ein LED-Zustand ist kein sicherheitsgerichteter Freigabeausgang und kein Nachweis für einen Resetpegel eines Aktors.

## Musterlösungen und PDF-Build

Alle Musterlösungen verwenden `::: {.content-visible when-profile="solution"}`. Die studentischen Startdateien enthalten bewusst noch keine Aufgabenlösung. Beispiel aus `labs/`:

```bash
quarto render lab_01_bare-metal-polling-interrupts.qmd --to pdf --profile solution
quarto render lab_01_bare-metal-polling-interrupts.qmd --to pdf
```

`build_labs.sh` erzeugt beide Fassungen unter `labs/_output/`; die kanonischen PDFs unter `labs/` bleiben studentische Fassungen. Projektanleitungen sind offen lesbare Dokumentation, keine zusätzlichen Pflichtblätter. Die PDFs verlinken sie auf GitHub, weil die Kurswebsite nicht automatisch die Markdown-Anleitungen veröffentlicht. Beide Fassungen verwenden helle Codeblöcke für den Druck.

Die [Probeklausur](probeklausur.qmd) bleibt bei etwa 60 Minuten und 60 Punkten: 20 Multiple-Choice-Fragen mit je vier Optionen, danach vier Papieraufgaben. Vortrag und Quiz werden durch diese Übungsüberarbeitung nicht geändert.
