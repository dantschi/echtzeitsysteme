# Projektvorbereitung für die Implementierungsaufgaben

Die folgenden Anleitungen beschreiben die vor dem Termin bereitzustellenden Projektbasen. Die Studierenden implementieren die im jeweiligen Übungsblatt angegebenen Funktionen selbst; eine vorbereitete Demonstration ist nicht das Arbeitsziel. Die Basis verwendet nur NUCLEO-H755ZI-Q, eingebaute LEDs/B1 und USB/ST-LINK. Alle Arbeitsaufträge stehen in den [Übungsblättern](../README.md). Blatt 3, 6 und 9 verwenden direkt die generierten CubeMX-Threadfunktionen und im Blatt abgedruckte Codebausteine.

| Blatt | Anleitung |
|---|---|
| 1 | [Taster-Nachleuchtanzeige ohne blockierendes Warten](lab_01_programmierung.md) |
| 2 | [Doppel-/Dreifachpuls als kooperativer Automat](lab_02_programmierung.md) |
| 3 | [zwei FreeRTOS-Threads mit LEDs: CubeMX-Konfiguration und Code im Blatt](lab_03_programmierung.md) |
| 4 | [Mutex-Änderung und konsistenter Snapshot](lab_04_programmierung.md) |
| 5 | [Taster-IRQ mit Wertqueue und LED-Worker](lab_05_programmierung.md) |
| 6 | [Polling-Server-Modell mit zwei CM4-Threads und LEDs](lab_06_programmierung.md) |
| 7 | [eigene geordnete M4-Antwort auf neue M7-Anfragen](lab_07_programmierung.md) |
| 8 | [Timing-Auswertung mit 64-Bit-Summe](lab_08_programmierung.md) |
| 9 | [begrenzte Verarbeitung mit Hysterese- und Eingabesequenzanzeige](lab_09_programmierung.md) |

STM32CubeMX und VS Code mit STM32 VS Code Extension müssen vorab eingerichtet sein. Projektgenerierung, Kernzuordnung, Flashstart, Linkerlayout und Bootfolge sind zu prüfen; die vorhandenen C-Dateien sind keine generierten Boardprojekte. Die Papieraufgaben bleiben ohne Hardware lösbar. Die vollständigen Musterfunktionen stehen nur im `solution`-Profil der Übungsblätter.
