# Echtzeitsysteme (EZS)

Herzlich willkommen zur Vorlesung **Echtzeitsysteme (EZS)** an der Dualen Hochschule Baden-Württemberg Stuttgart.

**Dozent:** Prof. Dr.-Ing. Daniel Klünder  
**Kurswebsite:** [dantschi.github.io/echtzeitsysteme](https://dantschi.github.io/echtzeitsysteme/)  
**Repository:** [github.com/dantschi/echtzeitsysteme](https://github.com/dantschi/echtzeitsysteme)

Das Modul vermittelt die Grundlagen **zeitkritischer Rechensysteme**: von harten und weichen Echtzeitanforderungen über Aufgabenmodelle und Scheduling bis zu Antwortzeitanalyse, Ressourcenprotokollen und Echtzeitbetriebssystemen. Sie lernen, Zeitverhalten systematisch zu spezifizieren, zu analysieren und in der Praxis mit einem RTOS umzusetzen — mit klarem Blick auf die Trennung von **Korrektheit der Funktion** und **Korrektheit im Zeitbereich**.

## Kursübersicht

Der rote Faden der Veranstaltung:

**Anforderungen → Aufgabenmodell → Scheduling → Analyse → Ressourcen → RTOS**

Pro Einheit: Folien (Reveal.js), Handout-PDF sowie Übungsblatt (Studierende / Musterlösung). Links verweisen auf die Kurswebsite. Weitere Einheiten werden semesterbegleitend ergänzt.

| Einheit | Thema | Schwerpunkt | Folien | Handout | Übungsblatt | Musterlösung |
|--------:|-------|-------------|:------:|:-------:|:-----------:|:------------:|
| 1 | Kickoff und Grundlagen | Harte/weiche Echtzeit, Deadlines, Determinismus | [HTML](https://dantschi.github.io/echtzeitsysteme/vorlesungen/01_kickoff.html) | [PDF](https://dantschi.github.io/echtzeitsysteme/vorlesungen/01_kickoff-handout.pdf) | — | — |
| 2 | Zeit und Aufgabenmodelle | Periodizität, WCET/BCET, Release/Deadline, Jitter | folgt | folgt | folgt | folgt |
| 3 | Scheduling-Grundlagen | Präemption, Prioritäten, Online/Offline, Metriken | folgt | folgt | folgt | folgt |
| 4 | Statisches Priority Scheduling | Rate Monotonic (RM), Deadline Monotonic (DM) | folgt | folgt | folgt | folgt |
| 5 | Dynamisches Scheduling | Earliest Deadline First (EDF), Optimalität | folgt | folgt | folgt | folgt |
| 6 | Schedulability und Response Time | Utilization Bound, Response-Time Analysis | folgt | folgt | folgt | folgt |
| 7 | Ressourcen und Synchronisation | Priority Inversion, PCP, Stack Resource Policy | folgt | folgt | folgt | folgt |
| 8 | Echtzeitbetriebssysteme | Tasks, Queues, ISRs, FreeRTOS-Praxis | folgt | folgt | folgt | folgt |
| 9 | Wrap-Up und Klausur | Kommunikation, Fallstricke, Prüfungstraining | folgt | folgt | folgt | folgt |

**Hinweise:** Folien im Browser öffnen (Speaker View: Taste **S**). Handout-PDF = Folieninhalt mit Skriptnotizen. Quelltexte unter [`vorlesungen/`](vorlesungen/) und [`labs/`](labs/).

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
| **FreeRTOS** | Praxisnahe Tasks, Queues, Semaphoren und Timing auf Mikrocontrollern | [freertos.org](https://www.freertos.org/) |
| **FreeRTOS Kernel Docs** | Referenz zu Scheduling, Interrupts und Synchronisation | [freertos.org/Documentation](https://www.freertos.org/Documentation/RTOS_book.html) |
| **Cheddar** | Scheduling-Simulation und Schedulability-Analysen zum Üben | [beru.univ-brest.fr/cheddar](http://beru.univ-brest.fr/~singhoff/cheddar/) |
| **Tracealyzer** (optional) | Laufzeit-Tracing von Tasks und Prioritätswechseln | [percepio.com/tracealyzer](https://percepio.com/tracealyzer/) |

## OER und Lizenz

Dieses Vorlesungsmaterial ist eine **Open Educational Resource (OER)**. Die Vorlesungsinhalte – Texte, Code und Diagramme – stehen unter der Lizenz **[Creative Commons Attribution 4.0 International (CC BY 4.0)](https://creativecommons.org/licenses/by/4.0/)**.

Sie dürfen diese Inhalte teilen und bearbeiten, sofern Sie die Urheberschaft angemessen nennen. Der vollständige Lizenztext liegt in der Datei [`LICENSE`](LICENSE) im Repository.

**Ausnahme:** Hochschullogos und andere geschützte Markenzeichen (einschließlich des DHBW-Logos) sind von dieser Lizenz **ausdrücklich ausgenommen**. Sie unterliegen dem Markenrecht der jeweiligen Rechteinhaber und dürfen ohne deren Genehmigung nicht übernommen oder weiterverwendet werden.
