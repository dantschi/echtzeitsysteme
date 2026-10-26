# Projektvorbereitung zu Übungsblatt 7

Verwenden Sie STM32CubeMX, VS Code mit STM32 VS Code Extension und **NUCLEO-H755ZI-Q / STM32H755ZIT6**. Die installierte Kombination muss die gewählten CM7-/CM4-CMake-Ziele unterstützen. Erhalten Sie die generierte Dual-Core-Boot-/HSEM-Startfolge und laden Sie zusammengehörige Images. C11 oder neuer; fremde Laborlast und doppelte Callbacks/Threadanlagen entfernen. Eigene Quellen und Benutzerbereiche müssen eine erneute CubeMX-Generierung mit anschließendem Build überstehen.

Die Programmieraufgabe wird von den Studierenden selbst implementiert. Bereiten Sie Installation, Boot, GPIO und gegebenenfalls Kernel, Linker und MPU vor dem Termin vor. Alle Versuche verwenden ausschließlich eingebaute LEDs, B1 und ST-LINK. Kein Sensor, Steckbrett, externer Widerstand, UART-Aufbau, Oszilloskop oder zusätzlicher Traceadapter ist erforderlich. Die Papieraufgaben sind unabhängig vom Aufbau lösbar; die Lehrperson kann aus dem Aufgabenpool auswählen.

Die folgenden Integrationsschritte gehören zur Vorbereitung der Lehrperson. Im Termin beurteilen die Studierenden die vereinbarten Adressen, Schreibrechte und Sichtbarkeitsbedingungen und implementieren den begrenzten Antwortschritt aus Aufgabe 6. Sie richten nicht parallel dazu Boot, Linker und MPU neu ein.

## Vorbereiteter Shared-Kanal

1. Verwenden Sie die Basisdateien `lab07_ipc.h`, `lab07_shared.c`, `lab07_cm7.c`, `lab07_cm4.c` und `shared-section.ld` aus `code/lab07/`. Dieses reale Modul behält sein **Dreiwortlayout** bei; der Vierwort-Papierentwurf aus Aufgabe 2 ist ausdrücklich ein anderes Beispiel.
2. In beiden tatsächlich verwendeten Linkerskripten SRAM4 ab `0x38000000` mit 64 KiB exklusiv reservieren; dort genau ein 32-Byte-ausgerichtetes `ipc` und eine 32-Byte-`NOLOAD`-Section. Andere Aliase, private Daten, Heap und Stacks dort entfernen. Beide Maps und Startup-Copy-/Zero-Tabellen prüfen; M4 und Debugger dürfen die initialisierte Shared-Section nach Freigabe nicht nullen.
3. Beim gemeinsamen Neustart vor M7-D-Cache-Aktivierung eine freie, effektiv vorrangige MPU-Region mit `Lab07_CM7_ConfigureMemory(region, LAB07_NONCACHED);` einrichten. Attribute Normal/TEX1, nicht cachebar, nicht bufferbar, Shareable, XN. Überlappende höhere Regionsnummern müssen den Vertrag erhalten. `region` ist eine tatsächlich freie Nummer, kein zu kopierender Platzhalterwert. Danach `Lab07_CM7_InitShared();`, D-Cache aktivieren und erst dann die **vorhandene** M4-Bootfreigabe ausführen. Keine zweite HSEM-Bootfolge ergänzen.
4. CM4 bekommt einen CMSIS-RTOS2-Worker (1000 Hz, 32-Bit-Ticks), der einmal `Lab07_CM4_Run(argument);` aufruft; keine anderen IPC-Nutzer. M7 ruft nach Bootfreigabe einmal `Lab07_CM7_Run();` auf. Seine HAL-Millisekundenzeit muss fortschreiten. Lokale Timeouts sind Diagnosepfade, keine automatische globale Eigentumsübernahme.
5. Prüfen Sie den vorbereiteten Alt-Basislauf zunächst **ohne** `LAB07_STUDENT_EXERCISE`: Antworten 1,3,5. Für die neue Aufgabe definieren Sie anschließend `LAB07_STUDENT_EXERCISE` in **beiden** CMake-Zielen und nehmen `respond_student.c` zusätzlich nur in CM4 auf. Erst dann gelten Anfragen 4,11,23 und Antworten 14,35,71. Eine einseitige Definition führt absichtlich zur Protokollabweichung. Der leere Rumpf erzeugt einen diagnostizierten Phasenfehler.
6. CM7 besitzt LD1/PB0 als `LAB07_RESULT`, Anfang LOW. Nach erfolgreichem Run, fehlerfreier Prüfung und gesetztem `lab07_cm7_done` im erhaltenen Benutzerbereich `HAL_GPIO_WritePin(LAB07_RESULT_GPIO_Port, LAB07_RESULT_Pin, GPIO_PIN_SET);` ausführen. M4 benutzt keinen LED-/Portkonfigurationscode für diesen Pin. Beide Done-Kennzeichen und alle Antworten im Debugger bei angehaltenen beiden Kernen prüfen.
7. Erneute Generierung und Build prüfen. Das 2000-Tick-/2000-ms-Wartebudget macht den Versuch endlich, beweist aber keine vollständige Inter-Core-Deadline. Keine cachebare Fehlerkonfiguration oder externe Zeitmessung ist für Aufgabe 6 erforderlich.

## Linkerreserve und Speicherattribute

Das vorhandene Fragment [`shared-section.ld`](../../code/lab07/shared-section.ld) wird in beide tatsächlich verwendeten Linkerdateien integriert. `RAM_SHARED` bezeichnet dort die einmal für den Kanal reservierte SRAM4-Region. Der zentrale Sektionsausschnitt lautet:

```ld
.shared ORIGIN(RAM_SHARED) (NOLOAD) :
{
  . = ALIGN(32);
  __lab07_shared_begin = .;
  KEEP(*(.shared .shared.*))
  __lab07_shared_data_end = .;
  . = ALIGN(32);
} > RAM_SHARED
```

`ORIGIN` setzt den Beginn an den Regionsursprung, `ALIGN(32)` richtet aus beziehungsweise polstert, und `KEEP` erhält die erfassten Eingabesektionen auch bei Entfernung unbenutzter Bereiche. `NOLOAD` legt keine normalen Initialisierungsdaten im Image an. Es verhindert keine späteren Startup- oder Debugger-Schreibzugriffe. Die vollständigen Prüfungen im Fragment verlangen die gemeinsame Adresse, zwölf Byte Objektinhalt und 32 Byte Sektionsreserve. Beide Maps und Kopier-/Nulltabellen sind zusätzlich zu prüfen; private Speicherbereiche dürfen SRAM4 nicht über einen zweiten Alias belegen.

Die vorbereitete M7-MPU-Region umfasst 64 KiB ab `0x38000000`, mit Lesen/Schreiben und Execute Never. Die folgenden Attribute gelten für den hier benutzten Cortex-M7-Entwurf:

| Attribut | Gültige Basis | Optionale Fehlervariante |
|---|---|---|
| Speichertyp | Normal, nicht cachebar | Normal, Write Back |
| TEX / C / B / S | 1 / 0 / 0 / 1 | 1 / 1 / 1 / 0 |
| Daten-Cache | darf nach Einrichtung aktiviert sein | muss tatsächlich aktiviert sein |
| Wartung des Kanals | keine private Cachekopie | absichtlich nicht ergänzt |

TEX bedeutet Type Extension, C cacheable, B bufferable und S shareable. Shareable erzeugt hier keine automatische Kohärenz zwischen M7 und M4. Eine freie, effektiv vorrangige Region wählen und alle aktivierten Überlappungen prüfen. Konfiguration und gemeinsame Initialisierung erfolgen bei ausgeschaltetem Daten-Cache vor der M4-Freigabe; Attribute werden nicht während laufender Kommunikation geändert. Der Fehlerbuild ist kein Teil der studentischen Implementierungsaufgabe. Ein beobachteter Erfolg dieses Builds belegt auch keine korrekte cachebare Kommunikation.

## Prüfstatus und Abgrenzung

Geben Sie einen vor dem Termin auf der tatsächlichen Hardware gebauten und gestarteten Basisstand aus. Die studentische Startdatei enthält bewusst noch keine Aufgabenlösung. Aufgaben und vollständige Musterfunktionen stehen im zugehörigen Übungsblatt, Lösungen nur im Quarto-Profil `solution`. Ein Source-/PDF-Build ist kein erfolgreicher Firmware- oder Boardtest. Die Cloudprüfung kann Anwendungslogik am Host prüfen, jedoch keine STM32-Takte, GPIO-/IRQ-Zuordnung, Speicherattribute oder reale Zeitgrenzen bestätigen. Ungemessene Zeiten bleiben offen.
