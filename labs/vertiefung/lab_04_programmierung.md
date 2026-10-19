# Projektvorbereitung zu Übungsblatt 4

Verwenden Sie STM32CubeMX, VS Code mit STM32 VS Code Extension und **NUCLEO-H755ZI-Q / STM32H755ZIT6**. Die installierte Kombination muss die gewählten CM7-/CM4-CMake-Ziele unterstützen. Erhalten Sie die generierte Dual-Core-Boot-/HSEM-Startfolge und laden Sie zusammengehörige Images. C11 oder neuer; fremde Laborlast und doppelte Callbacks/Threadanlagen entfernen. Eigene Quellen und Benutzerbereiche müssen eine erneute CubeMX-Generierung mit anschließendem Build überstehen.

Die Programmieraufgabe wird von den Studierenden selbst implementiert. Bereiten Sie Installation, Boot, GPIO und gegebenenfalls Kernel, Linker und MPU vor dem Termin vor. Alle Versuche verwenden ausschließlich eingebaute LEDs, B1 und ST-LINK. Kein Sensor, Steckbrett, externer Widerstand, UART-Aufbau, Oszilloskop oder zusätzlicher Traceadapter ist erforderlich. Die Papieraufgaben sind unabhängig vom Aufbau lösbar; die Lehrperson kann aus dem Aufgabenpool auswählen.

## Ressourcenstand auf CM4

FreeRTOS ausschließlich auf CM4, CMSIS_V2, Präemption, 32-Bit-Kernelticks und 1000 Hz. Verwenden Sie SysTick für den Kernel und einen freien TIM6 für die HAL-Zeitbasis; erhalten Sie die generierten Handlerketten. Heap und tatsächliche Stacks müssen auch Idle, optionale Timer-Task und Verwaltungsanteile abdecken; 1024 Byte je hier angelegtem Thread sind Ausgangsgrößen, keine bewiesenen Minima. Rufen Sie den angegebenen Starthelfer genau einmal nach dem vorhandenen `osKernelInitialize()` im Zustand `osKernelReady` und vor dem vorhandenen `osKernelStart()` auf. Legen Sie seine Threads nicht nochmals in CubeMX an. Entfernen/deaktivieren Sie nicht benötigte Default-Threads und frühere Anwendungsmodule. CM7 bleibt beim generierten Systemstart und einer ruhigen Hauptschleife.

1. LD1/PB0 `LAB04_GOOD`, LD2/PE1 `LAB04_STATUS`, LD3/PB14 `LAB04_BAD`: Ausgänge, anfangs LOW, CM4 alleiniger GPIO-Besitzer. Belegung und aktive Polarität gegen UM2408/Boardrevision prüfen.
2. `lab04_account.c`, `account_student.c` und `lab04_account.h` ausschließlich in das CM4-Ziel aufnehmen. Den alten Inversionsdemonstrator `lab04_resources.c` nicht parallel einbinden.
3. `lab04_account.h` im Benutzer-Includebereich ergänzen und an der oben beschriebenen RTOS-Stelle `Lab04_AccountStart();` aufrufen. Der Helper legt einen vererbenden Mutex, zwei Writer mit Normal und einen Reader mit AboveNormal an.
4. Die vorbereiteten Writer verwenden dauerhaft gültige Argumente und je 100 begrenzte Änderungen. `done` wird ebenfalls unter dem gemeinsamen Mutex aktualisiert. LED- und Warteaufrufe des Readers liegen außerhalb der studentischen Snapshotfunktion.
5. Mit den noch leeren studentischen Funktionen startet der Kernel, die Status-LED läuft, die Fehleranzeige bleibt aktiv; das ist noch kein erfolgreicher Anwendungsnachweis. Nach Implementierung: konsistente Paare, `done == 2`, Endstand 1100 und Prüffeld 2201 prüfen. Diagnose im angehaltenen CM4 lesen. Absichtlich ungeschützte C-Datenrennen sind kein Boardversuch.

## Prüfstatus und Abgrenzung

Geben Sie einen vor dem Termin auf der tatsächlichen Hardware gebauten und gestarteten Basisstand aus. Die studentische Startdatei enthält bewusst noch keine Aufgabenlösung. Aufgaben und vollständige Musterfunktionen stehen im zugehörigen Übungsblatt, Lösungen nur im Quarto-Profil `solution`. Ein Source-/PDF-Build ist kein erfolgreicher Firmware- oder Boardtest. Die Cloudprüfung kann Anwendungslogik am Host prüfen, jedoch keine STM32-Takte, GPIO-/IRQ-Zuordnung, Speicherattribute oder reale Zeitgrenzen bestätigen. Ungemessene Zeiten bleiben offen.
