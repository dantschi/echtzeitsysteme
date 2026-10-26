# Echtzeitsysteme (EZS)

Herzlich willkommen zur Vorlesung **Echtzeitsysteme (EZS)** an der Dualen Hochschule Baden-Württemberg Stuttgart, Studiengang Elektro- und Informationstechnik.

**Dozent:** Prof. Dr.-Ing. Daniel Klünder  
**Kurswebsite:** [dantschi.github.io/echtzeitsysteme](https://dantschi.github.io/echtzeitsysteme/)  
**Repository:** [github.com/dantschi/echtzeitsysteme](https://github.com/dantschi/echtzeitsysteme)

Das Modul vermittelt die Grundlagen **zeitkritischer Rechensysteme**: von harten und weichen Echtzeitanforderungen über Task-Modelle und Scheduling (RMS/EDF) bis zu Echtzeitbetriebssystemen mit **FreeRTOS**, Synchronisation, Inter-Task-Kommunikation, Server-Verfahren für ereignisgetriebene Last, Multicore-Architekturen (AMP) am **STM32H755** sowie Timing-Analyse. Den Abschluss bildet eine integrierte Fallstudie zu Zeit- und Datenverträgen, Fortschrittsüberwachung und begründeter Verifikation. Sie lernen, Zeitverhalten systematisch zu spezifizieren, zu analysieren und in der Praxis umzusetzen — mit klarem Blick auf die Trennung von **funktioneller Korrektheit** und **Korrektheit im Zeitbereich**.

## Kursübersicht

Der rote Faden der Veranstaltung:

**Anforderungen → Scheduling → RTOS → Kommunikation → Server-Verfahren → Multicore → Timing-Analyse → Systemprüfung**

Pro Einheit: Folien (Reveal.js), Handout-PDF, Übungsblatt (Studierende / Musterlösung) und Implementierungshilfe mit Projektanleitung sowie je nach Übung Codebausteinen im Blatt oder Startdateien. Folien und PDFs liegen auf der Kurswebsite, Anleitungen und Code im Repository.

| Einheit | Thema | Schwerpunkt | Folien | Handout | Übungsblatt | Musterlösung | Implementierungshilfe |
|--------:|-------|-------------|:------:|:-------:|:-----------:|:------------:|:---------------------:|
| 1 | Was ist Echtzeit | Definition, Latenz/Jitter, Polling, Interrupts | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/01_was-ist-echtzeit.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/01_was-ist-echtzeit-handout.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_01_bare-metal-polling-interrupts.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_01_bare-metal-polling-interrupts-musterloesung.pdf) | [Anleitung](https://github.com/dantschi/echtzeitsysteme/blob/main/labs/vertiefung/lab_01_programmierung.md) · [Code](https://github.com/dantschi/echtzeitsysteme/tree/main/code/lab01) |
| 2 | Scheduling-Theorie | Task-Modell, Utilization, RMS/EDF, kooperativer Scheduler | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/02_scheduling-theorie.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/02_scheduling-theorie-handout.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_02_scheduling-theorie-kooperativer-scheduler.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_02_scheduling-theorie-kooperativer-scheduler-musterloesung.pdf) | [Anleitung](https://github.com/dantschi/echtzeitsysteme/blob/main/labs/vertiefung/lab_02_programmierung.md) · [Code](https://github.com/dantschi/echtzeitsysteme/tree/main/code/lab02) |
| 3 | Einführung Echtzeitbetriebssysteme | FreeRTOS, TCB/Stack, Präemption, Starvation, CMSIS | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/03_einfuehrung-rtos.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/03_einfuehrung-rtos-handout.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_03_einfuehrung-freertos-praeemption.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_03_einfuehrung-freertos-praeemption-musterloesung.pdf) | [Anleitung](https://github.com/dantschi/echtzeitsysteme/blob/main/labs/vertiefung/lab_03_programmierung.md) · [Code im Blatt](https://github.com/dantschi/echtzeitsysteme/blob/main/labs/lab_03_einfuehrung-freertos-praeemption.qmd#aufgabe-6-zwei-freertos-threads-mit-leds) |
| 4 | Synchronisation und Ressourcenverwaltung | Race Conditions, Mutex, Deadlocks, Priority Inversion | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/04_synchronisation-ressourcen.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/04_synchronisation-ressourcen-handout.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_04_race-conditions-mutex-priority-inversion.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_04_race-conditions-mutex-priority-inversion-musterloesung.pdf) | [Anleitung](https://github.com/dantschi/echtzeitsysteme/blob/main/labs/vertiefung/lab_04_programmierung.md) · [Code](https://github.com/dantschi/echtzeitsysteme/tree/main/code/lab04) |
| 5 | Inter-Task-Kommunikation und Interrupts | Queues, Deferred Interrupt Processing (Top-/Bottom-Half) | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/05_inter-task-kommunikation.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/05_inter-task-kommunikation-handout.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_05_deferred-interrupt-processing.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_05_deferred-interrupt-processing-musterloesung.pdf) | [Anleitung](https://github.com/dantschi/echtzeitsysteme/blob/main/labs/vertiefung/lab_05_programmierung.md) · [Code](https://github.com/dantschi/echtzeitsysteme/tree/main/code/lab05) |
| 6 | Server-Verfahren für ereignisgetriebene Last | CPU-Budget, Polling, Deferrable, Sporadic, Überlast | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/06_server-verfahren.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/06_server-verfahren-handout.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_06_server-verfahren.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_06_server-verfahren-musterloesung.pdf) | [Anleitung](https://github.com/dantschi/echtzeitsysteme/blob/main/labs/vertiefung/lab_06_programmierung.md) · [Code im Blatt](https://github.com/dantschi/echtzeitsysteme/blob/main/labs/lab_06_server-verfahren.qmd#aufgabe-6-einen-polling-server-als-led-modell-selbst-simulieren) |
| 7 | Multicore-Echtzeit | AMP vs. SMP, Shared Memory, Cache-Kohärenz, MPU | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/07_multicore-echtzeit.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/07_multicore-echtzeit-handout.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_07_multicore-shared-memory-cache-kohaerenz.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_07_multicore-shared-memory-cache-kohaerenz-musterloesung.pdf) | [Anleitung](https://github.com/dantschi/echtzeitsysteme/blob/main/labs/vertiefung/lab_07_programmierung.md) · [Code](https://github.com/dantschi/echtzeitsysteme/tree/main/code/lab07) |
| 8 | Timing-Analyse und Messmethoden | Messvertrag, Beobachtereffekt, CPU-/Antwortzeit, Nachweisgrenzen | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/08_tracing-timing-analyse.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/08_tracing-timing-analyse-handout.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_08_tracing-timing-analyse.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_08_tracing-timing-analyse-musterloesung.pdf) | [Anleitung](https://github.com/dantschi/echtzeitsysteme/blob/main/labs/vertiefung/lab_08_programmierung.md) · [Code](https://github.com/dantschi/echtzeitsysteme/tree/main/code/lab08) |
| 9 | Integrierte Fallstudie und Verifikation | Polling, Antwortzeiten, Datenvertrag, Fortschritt, Watchdog | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/09_capstone-architekturmuster.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/09_capstone-architekturmuster-handout.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_09_capstone-timing-verifikation.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/lab_09_capstone-timing-verifikation-musterloesung.pdf) | [Anleitung](https://github.com/dantschi/echtzeitsysteme/blob/main/labs/vertiefung/lab_09_programmierung.md) · [Code im Blatt](https://github.com/dantschi/echtzeitsysteme/blob/main/labs/lab_09_capstone-timing-verifikation.qmd) |

**Hinweise:** Folien im Browser öffnen (Speaker View: Taste **S**). Handout-PDF = Folieninhalt mit Skriptnotizen. Quelltexte unter [`vorlesungen/`](vorlesungen/) und [`labs/`](labs/).

## Probeklausur

| Dokument | Studierende | Musterlösung |
|----------|:-----------:|:------------:|
| Probeklausur (60 Min., 60 Punkte: 20 MC-Fragen und Papieraufgaben) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/probeklausur.pdf) | [PDF](https://dantschi.github.io/echtzeitsysteme/labs/probeklausur-musterloesung.pdf) |

Quelltext: [`labs/probeklausur.qmd`](labs/probeklausur.qmd).

## Literatur

Zentrale Referenz dieses Moduls:

> **Giorgio C. Buttazzo**  
> *Hard Real-Time Computing Systems: Predictable Scheduling Algorithms and Applications*  
> Springer

Definitionen, Terminologie und didaktischer Aufbau orientieren sich an diesem Standardwerk.

Zur Vertiefung empfohlen:

> **Hermann Kopetz**  
> *Real-Time Systems: Design Principles for Distributed Embedded Applications*  
> Springer

## Tools und Ressourcen

| Tool | Einsatz | Link |
|------|---------|------|
| **FreeRTOS** | Tasks, Queues, Semaphoren und Timing auf Mikrocontrollern | [freertos.org](https://www.freertos.org/) |
| **FreeRTOS Kernel Docs** | Referenz zu Scheduling, Interrupts und Synchronisation | [freertos.org/Documentation](https://www.freertos.org/Documentation/RTOS_book.html) |
| **STM32CubeMX** | Board, Pins, Takte, FreeRTOS und CMake-Projekt konfigurieren | [st.com/stm32cubemx](https://www.st.com/en/development-tools/stm32cubemx.html) |
| **VS Code mit STM32 VS Code Extension** | STM32-Firmware bauen, laden und debuggen | [ST-Dokumentation](https://www.st.com/en/development-tools/stm32-vscode-extension.html) |
| **TASKING BlueBox / winIDEA** (optional) | zusätzlicher geprüfter Debug-/Tracepfad, keine Voraussetzung der Übungen | [tasking.com](https://www.tasking.com/) |
| **Cheddar** | Scheduling-Simulation und Schedulability-Analysen zum Üben | [beru.univ-brest.fr/cheddar](http://beru.univ-brest.fr/~singhoff/cheddar/) |
| **Tracealyzer** (optional) | Laufzeit-Tracing von Tasks und Prioritätswechseln | [percepio.com/tracealyzer](https://percepio.com/tracealyzer/) |

## OER und Lizenz

Dieses Vorlesungsmaterial ist eine **Open Educational Resource (OER)**. Die Vorlesungsinhalte – Texte, Code und Diagramme – stehen unter der Lizenz **[Creative Commons Attribution 4.0 International (CC BY 4.0)](https://creativecommons.org/licenses/by/4.0/)**.

Sie dürfen diese Inhalte teilen und bearbeiten, sofern Sie die Urheberschaft angemessen nennen. Der vollständige Lizenztext liegt in der Datei [`LICENSE`](LICENSE) im Repository.

**Ausnahme:** Hochschullogos und andere geschützte Markenzeichen (einschließlich des DHBW-Logos) sind von dieser Lizenz **ausdrücklich ausgenommen**. Sie unterliegen dem Markenrecht der jeweiligen Rechteinhaber und dürfen ohne deren Genehmigung nicht übernommen oder weiterverwendet werden.
