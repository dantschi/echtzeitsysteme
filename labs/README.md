# Übungsblätter: Papierklausur und Praxis

Die Modulnote entsteht durch eine Papierklausur. Die Übungen bereiten deshalb vor allem das Rechnen, Zeichnen, Lesen kleiner Codeausschnitte und das begründete Beurteilen von Entwürfen vor. Eine erfolgreiche Firmwareintegration ist keine Voraussetzung für das Bearbeiten des nächsten Blatts. Die Punkte auf den Blättern dienen der Selbsteinschätzung, nicht als zusätzliche Programmierbewertung.

**Blatt 2 und 3 beginnen unmittelbar mit neuen, eigenständig lösbaren Aufgaben und enthalten jeweils eine kleine Programmieraufgabe für die Studierenden.** In Blatt 2 implementieren sie eine kooperative Signalfolge, in Blatt 3 eine Füllstandsüberwachung mit zwei RTOS-Threads. Die Projektbasis wird vorbereitet; die Anwendungslogik schreiben die Studierenden selbst. Der Aufgabenpool darf länger als der verfügbare Block sein, die Lehrperson wählt passende Aufgaben aus. Die folgenden Vorgaben für kurze Demonstrationen gelten für die übrigen Blätter.

Die übrigen Blätter haben einen verbindlichen Papierkern, einen Transferfall mit Besprechung und einen Praxisfall von 15 Minuten. Der Praxisfall enthält alle Daten für eine Papieralternative. Eine kurze vorbereitete Demonstration kann denselben Begriff anschaulich machen. An einer bereits geprüften Station können Kleingruppen den Vergleich auch selbst ausführen; Aufbau und Integration gehören weiterhin in die Vorbereitung. Auch bei vollständig fehlender Hardware bleiben alle Lernziele bearbeitbar.

## Prüfung der Blätter und Änderungen

| Blatt | Papierkern für die Klausur | Bisheriger Aufwand im Praxisteil | Entscheidung für den Übungstermin |
|---|---|---|---|
| [1](lab_01_bare-metal-polling-interrupts.qmd) | Zeitvertrag, Echtzeitklasse, Latenz/Jitter, Polling, IRQ, Timer | vier Varianten und Messprotokoll, 35 min | 40 min Kern + 20 min Transfer; kurzer Polling-/Timervergleich oder Zeitachse, 15 min |
| [2](lab_02_scheduling-theorie-kooperativer-scheduler.qmd) | neue Beispiele zu Jobmodell, RMS, Antwortzeitanalyse, EDF, kooperativen Runden | eigener Signalautomat auf vorbereiteter Basis | fünf Papieraufgaben und eigene Doppelpuls-/Dreifachpulsimplementierung, ca. 25–35 min Praxis; Auswahl möglich |
| [3](lab_03_einfuehrung-freertos-praeemption.qmd) | neue Beispiele zu Zuständen, Präemption, Tickphase, Drift, Speicher, Starvation, Flags | zwei eigene Threadfunktionen auf vorbereiteter Basis, kein IRQ-/ADC-Setup | fünf Papieraufgaben und eigene Mess-/Signalimplementierung, ca. 30–40 min Praxis; Auswahl möglich |
| [4](lab_04_race-conditions-mutex-priority-inversion.qmd) | Lost Update, Schutzumfang, Semaphor, Deadlock, Priority Inversion | Integration und zwei Varianten, 35 min | 50 min Kern + 20 min UART-/Besitztransfer; Semaphor-/Mutexreihenfolge und Marker erklären, 15 min |
| [5](lab_05_deferred-interrupt-processing.qmd) | Ringzustände, Kapazität, Wert/Pointer, IRQ-Übergabe | Wertqueue und zusätzlich eigener Button-/EXTI-Versuch | 50 min Kern + 20 min Puffertransfer; abgeschlossenen Queue-Log lesen, 15 min; Buttonaufbau freiwillig |
| [6](lab_06_multicore-shared-memory-cache-kohaerenz.qmd) | AMP, Zeitbudget, Layout, Start, Besitz, Cache, MPU | neues Linker-/MPU-Layout und zwei Dual-Core-Builds, 35 min | 50 min Kern + 20 min Entwurfsreview; Cache und SRAM anhand vorgegebener Sichten vergleichen, 15 min |
| [7](lab_07_tracing-timing-analyse.qmd) | Messvertrag, Antwortzeit, Modulozähler, Beobachtereffekt, Tracequalität | Zählermodul und zusätzliche Traceoption, 30 min | 50 min Kern + 15 min Nachweisreview; Zählerwerte auswerten, 15 min; keine neue Tracehardware im Pflichtteil |
| [8](lab_08_hsem-wwdg-safe-shutdown.qmd) | HSEM, Lost Wakeup, Restbudget, Watchdogfenster, Ausgangsschutz | neuer Shared-Block plus optionale WWDG-Station | 50 min Kern + 10 min Reset-/Boottransfer; genau ein vorbereiteter Vergleich oder Papierfall, 15 min |
| [9](lab_09_capstone-timing-verifikation.qmd) | Architektur, Verlustbilanzen, Zeitabnahme, Filter, Fortschritt | integrierte Dual-Core-/Timer-/Mailboxkette mit weiteren Varianten | 50 min Kern + 10 min Abnahmereview; Papier-Capstone oder fertiger Sättigungslauf, 15 min |

Blatt 5 hat 30 Orientierungspunkte, die anderen Blätter jeweils 32. Blatt 2 und 3 verwenden neue Papieraufgaben und Musterlösungen; zusätzliche Verständnis- und Programmieraufgaben haben keine weiteren Punkte. Bei diesen beiden Blättern gibt es keine feste Obergrenze für den gesamten Aufgabenpool. Die übrigen Übungsblöcke belegen 75 bis 85 Minuten innerhalb der jeweiligen 180-Minuten-Veranstaltung. In den Folien stehen teilweise noch die bisherigen Integrationszeiten; für die neue Übungsorganisation gelten die Zeitpläne der Blätter. Vortrag und Quiz wurden bei dieser Überarbeitung nicht geändert.

## Ablauf für die kurzen Praxisfälle der übrigen Blätter

1. Papieraufgaben einzeln oder zu zweit bearbeiten, jeweils nach Erklärung der benötigten Begriffe. Zeitachsen beschriften, Einheiten nennen und Rechenwege sichtbar machen.
2. Im Transferfall zunächst selbst entscheiden und anschließend die Begründungen vergleichen. Besonders prüfen: Welche Voraussetzung ist erfüllt, welche Aussage ist tatsächlich bewiesen?
3. Den Praxisfall vorhersagen, eine kurze vorbereitete Demonstration betrachten und danach den Mechanismus erklären. Richtwert: fünf Minuten je Phase. Ohne Aufbau wird der vorgegebene Papierfall vollständig bearbeitet.
4. Nach spätestens drei Minuten technischer Fehlersuche zum Papierfall wechseln. Die Fehlersuche wird außerhalb des Übungstermins fortgesetzt. Studierende warten nicht auf einen erfolgreichen Build anderer Gruppen.
5. Zum Abschluss die begründeten Lösungen sichern. Gemessene Werte, Lehrannahmen, offene Prüfungen und tatsächliche Fehler werden getrennt benannt.

## Vorbereitung der Lehrperson

Die Programmieraufgaben und Demonstrationen verwenden STM32CubeMX und VS Code mit STM32 VS Code Extension auf dem NUCLEO-H755ZI-Q / STM32H755ZIT6. Geeignete Projektvarianten werden vor dem Termin erzeugt, gebaut, auf dem tatsächlichen Board gestartet und auf das erwartete Verhalten geprüft. Die C-Module unter `code/` ersetzen kein vollständiges Firmwareprojekt. Dieses Repository verspricht weder einen bereits getesteten Gesamtaufbau noch feste reale Laufzeiten.

Vor einer Programmieraufgabe oder Demonstration: Boardrevision und Werkzeugstände festhalten, die richtige Kern-/Imagezuordnung und Startfolge prüfen, alte Laborlast entfernen und den passenden Daten-/Zeitbasisvertrag kontrollieren. Für Blatt 6, 8 und 9 sind Linker-/Startup-/MPU-Änderungen vollständig vorbereitet. Für Blatt 7 bleibt zusätzliche Tracehardware optional. Wenn kein geprüftes Projekt verfügbar ist, genügt der Papierfall; die Durchführung des Unterrichts hängt nicht vom Aufbau ab.

Die ausführlichen Anleitungen liegen unter [vertiefung/](vertiefung/). Für Blatt 2 und 3 beschreiben sie die Vorbereitung der studentischen Projektbasis. Bei den übrigen Blättern dienen sie der Demonstrationsvorbereitung und freiwilliger Praxis außerhalb der Unterrichtszeit. Die jeweilige Anleitung benennt weiterhin Voraussetzungen, vollständige Anwendungsmodule und Fehlergrenzen. Eigene Integrationsarbeit wird nicht in den 15-Minuten-Praxisfall hineingerechnet.

## Musterlösungen und Probeklausur

Alle Lösungen im Übungsblatt stehen in `::: {.content-visible when-profile="solution"}`. Die studentische PDF enthält die Aufgaben, die Lösungsversion zusätzlich den Erwartungshorizont. Beispiel aus dem Verzeichnis `labs/`:

```bash
quarto render lab_01_bare-metal-polling-interrupts.qmd --to pdf --profile solution
quarto render lab_01_bare-metal-polling-interrupts.qmd --to pdf
```

`build_labs.sh` erzeugt beide Fassungen unter `labs/_output/`; die kanonischen PDFs unter `labs/` bleiben studentische Fassungen. Die Markdown-Anleitungen unter `vertiefung/` sind offen lesbare technische Dokumentation; die Arbeitsaufträge stehen auf den Übungsblättern. Die PDFs verlinken diese Anleitungen auf GitHub, weil die Kurswebsite die Labor-PDFs, aber nicht automatisch die Markdown-Anleitungen veröffentlicht. Beide PDF-Fassungen verwenden helle Codeblöcke für die Druckausgabe.

Die [Probeklausur](probeklausur.qmd) ist auf etwa 60 Minuten und 60 Punkte ausgelegt: 20 echte Multiple-Choice-Fragen mit je vier Optionen und einer, mehreren oder allen richtigen Antworten (20 Punkte, etwa 20 Minuten), danach vier Papieraufgaben zu Scheduling, Prioritätsinversion, Kommunikation/Speicher und Timing/Fehlerreaktion (je 10 Punkte und etwa 10 Minuten). Im Auswahlteil zählt ausschließlich die vollständige richtige Antwortkombination. Die Papieraufgaben erlauben methodische Teilpunkte. Hilfsmittel sind ein einfacher Taschenrechner ohne weitere Unterlagen; Formeln und Modellannahmen sind angegeben. Die Musterlösung enthält alle Antwortkombinationen, Begründungen und Teilpunkteschlüssel.
