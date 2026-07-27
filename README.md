# Echtzeitsysteme (EZS)

Herzlich willkommen zur Vorlesung **Echtzeitsysteme (EZS)** an der Dualen Hochschule Baden-Württemberg Stuttgart, Studiengang Elektro- und Informationstechnik.

**Dozent:** Prof. Dr.-Ing. Daniel Klünder  
**Kurswebsite:** [dantschi.github.io/echtzeitsysteme](https://dantschi.github.io/echtzeitsysteme/)  
**Repository:** [github.com/dantschi/echtzeitsysteme](https://github.com/dantschi/echtzeitsysteme)

Das Modul vermittelt die Grundlagen **zeitkritischer Rechensysteme**: von harten und weichen Echtzeitanforderungen über Task-Modelle und Scheduling (RMS/EDF) bis zu Echtzeitbetriebssystemen mit **FreeRTOS**, Synchronisation, Inter-Task-Kommunikation und Multicore-Architekturen (AMP) am Beispiel des **STM32H755**. Sie lernen, Zeitverhalten systematisch zu spezifizieren, zu analysieren und in der Praxis umzusetzen — mit klarem Blick auf die Trennung von **funktioneller Korrektheit** und **Korrektheit im Zeitbereich**.

## Kursübersicht

Der rote Faden der Veranstaltung:

**Anforderungen → Scheduling → RTOS → Sync/IPC → Multicore → Tracing → Schedulability**

Pro Einheit: Folien (Reveal.js) und Handout-PDF. Links verweisen auf die Kurswebsite. Weitere Einheiten und Übungsblätter werden semesterbegleitend ergänzt.

| Einheit | Thema | Schwerpunkt | Folien | Handout |
|--------:|-------|-------------|:------:|:-------:|
| 1 | Was ist Echtzeit | Definition, Latenz/Jitter, Polling, Interrupts, Paradigmenwechsel | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/01_was-ist-echtzeit.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/01_was-ist-echtzeit-handout.pdf) |
| 2 | Scheduling-Theorie | Task-Modell, Utilization, RMS/EDF, SysTick/PendSV, Round-Robin | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/02_scheduling-theorie.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/02_scheduling-theorie-handout.pdf) |
| 3 | Einführung Echtzeitbetriebssysteme | FreeRTOS, TCB/Stack, Zustände, Tickless/CMSIS, Labor STM32 | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/03_einfuehrung-rtos.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/03_einfuehrung-rtos-handout.pdf) |
| 4 | Synchronisation und Ressourcenverwaltung | Race Conditions, Semaphoren, Mutexe, Deadlocks, Priority Inversion | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/04_synchronisation-ressourcen.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/04_synchronisation-ressourcen-handout.pdf) |
| 5 | Inter-Task-Kommunikation und Interrupts | Queues, Mailboxes, Copy by Value/Reference, ISR-Anbindung | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/05_inter-task-kommunikation.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/05_inter-task-kommunikation-handout.pdf) |
| 6 | Multicore-Echtzeit | AMP vs. SMP, STM32H755 Dual-Core, Shared Memory, Cache/MPU | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/06_multicore-echtzeit.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/06_multicore-echtzeit-handout.pdf) |
| 7 | Tracing & Timing-Analyse | Non-Intrusive Observation, CoreSight, BlueBox | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/07_tracing-timing-analyse.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/07_tracing-timing-analyse-handout.pdf) |
| 8 | Multicore-Synchronisation & Safety | HSEM, IWDG/WWDG (EWI), MPU-Safety, HSEM-Labor | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/08_multicore-synchronisation-safety.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/08_multicore-synchronisation-safety-handout.pdf) |
| 9 | Capstone-Projekt & Architekturmuster | Edge, Fail-Safe/Operational, AUTOSAR, Capstone | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/09_capstone-architekturmuster.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/09_capstone-architekturmuster-handout.pdf) |

**Hinweise:** Folien im Browser öffnen (Speaker View: Taste **S**). Handout-PDF = Folieninhalt mit Skriptnotizen. Quelltexte unter [`vorlesungen/`](vorlesungen/).

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
| **STM32CubeIDE** | Entwicklung und Debug am STM32 (u. a. STM32H755 Dual-Core) | [st.com/stm32cubeide](https://www.st.com/en/development-tools/stm32cubeide.html) |
| **Cheddar** | Scheduling-Simulation und Schedulability-Analysen zum Üben | [beru.univ-brest.fr/cheddar](http://beru.univ-brest.fr/~singhoff/cheddar/) |
| **Tracealyzer** (optional) | Laufzeit-Tracing von Tasks und Prioritätswechseln | [percepio.com/tracealyzer](https://percepio.com/tracealyzer/) |

## OER und Lizenz

Dieses Vorlesungsmaterial ist eine **Open Educational Resource (OER)**. Die Vorlesungsinhalte – Texte, Code und Diagramme – stehen unter der Lizenz **[Creative Commons Attribution 4.0 International (CC BY 4.0)](https://creativecommons.org/licenses/by/4.0/)**.

Sie dürfen diese Inhalte teilen und bearbeiten, sofern Sie die Urheberschaft angemessen nennen. Der vollständige Lizenztext liegt in der Datei [`LICENSE`](LICENSE) im Repository.

**Ausnahme:** Hochschullogos und andere geschützte Markenzeichen (einschließlich des DHBW-Logos) sind von dieser Lizenz **ausdrücklich ausgenommen**. Sie unterliegen dem Markenrecht der jeweiligen Rechteinhaber und dürfen ohne deren Genehmigung nicht übernommen oder weiterverwendet werden.
