# Projektvorbereitung für die Implementierungsaufgaben

Die folgenden Anleitungen beschreiben die vor dem Termin bereitzustellenden Projektbasen. Die Studierenden implementieren die im jeweiligen Übungsblatt angegebenen Funktionen selbst; eine vorbereitete Demonstration ist nicht das Arbeitsziel. Die Basis verwendet nur NUCLEO-H755ZI-Q, eingebaute LEDs/B1 und USB/ST-LINK. Alle Arbeitsaufträge stehen in den [Übungsblättern](../README.md).

| Blatt | Anleitung |
|---|---|
| 1 | [Taster-Nachleuchtanzeige ohne blockierendes Warten](lab_01_programmierung.md) |
| 2 | [Doppel-/Dreifachpuls als kooperativer Automat](lab_02_programmierung.md) |
| 3 | [HeavyMath, Heartbeat und Tasterreaktion mit FreeRTOS nur auf M4](lab_03_programmierung.md) |
| 4 | [Mutex-Änderung und konsistenter Snapshot](lab_04_programmierung.md) |
| 5 | [Taster-IRQ mit Wertqueue und LED-Worker](lab_05_programmierung.md) |
| 6 | [eigene geordnete M4-Antwort auf neue M7-Anfragen](lab_06_programmierung.md) |
| 7 | [robuste Gruppenauswertung mit 64-Bit-Summe](lab_07_programmierung.md) |
| 8 | [Frischeprüfung eines Supervisors mit Fehlerprovokation über B1](lab_08_programmierung.md) |
| 9 | [Hysterese- und Publikationslückenanzeige im Capstone](lab_09_programmierung.md) |

STM32CubeMX und VS Code mit STM32 VS Code Extension müssen vorab eingerichtet sein. Projektgenerierung, Kernzuordnung, Flashstart, Linkerlayout und Bootfolge sind zu prüfen; die vorhandenen C-Dateien sind keine generierten Boardprojekte. Die Papieraufgaben bleiben ohne Hardware lösbar. Die vollständigen Musterfunktionen stehen nur im `solution`-Profil der Übungsblätter.
