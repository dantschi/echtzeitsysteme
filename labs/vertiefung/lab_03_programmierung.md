# Projektvorbereitung zu Übungsblatt 3

Die Studierenden implementieren in Aufgabe 6 zwei eigene Threadfunktionen für eine simulierte Füllstandsüberwachung. Diese Anleitung beschreibt die vorher einzurichtende Projektbasis. ADC, Taster, EXTI und zusätzliche Messhardware sind dafür nicht erforderlich.

## Basis vor dem Termin bereitstellen

1. Verwenden Sie **STM32CubeMX**, **VS Code mit STM32 VS Code Extension** und **NUCLEO-H755ZI-Q / STM32H755ZIT6**. Benötigt wird ein Dual-Core-CMake-Projekt mit CM7-/CM4-Zielen und funktionierender generierter Boot-/HSEM-Startfolge. Die installierte Werkzeugkombination muss insbesondere FreeRTOS auf CM4 mit diesem Generator unterstützen. CM7 behält Systemstart und gemeinsame Taktkonfiguration; beide Images müssen zusammenpassen.
2. Konfigurieren Sie FreeRTOS ausschließlich auf **CM4**, **CMSIS_V2**, Präemption, 32-Bit-Kernelticks und **1000 Hz**. Prüfen Sie die tatsächlich erzeugte `FreeRTOSConfig.h` und CMSIS-Prioritätsabbildung. Die HAL erhält in dieser Projektvariante einen freien **TIM6**, der Kernel SysTick. Erhalten Sie die generierten Handlerketten; prüfen Sie beide Zeitbasen.
3. Konfigurieren Sie **PB0 / LD1** als Ausgang auf CM4, User Label `TANK_LED`, anfangs LOW. Prüfen Sie Boardrevision, Pin und aktive Polarität anhand UM2408. CM7 darf diesen Pin weder konfigurieren noch schreiben. Entfernen Sie frühere Anwendungen, EXTI-Callbacks und dauerhaft bereite Testlast aus dem aktiven CM4-Projekt.
4. Nehmen Sie `lab03_tank.c`, die studentische Startdatei `tank_threads.c` und deren Headerpfad aus `code/lab03/` in das **CM4-CMake-Ziel** auf; C11 oder neuer verwenden. `lab03_rtos.c` gehört zum früheren Demonstrator und wird hier nicht benötigt. Die beiden neuen Threads werden im Modul angelegt, nicht nochmals in CubeMX. Entfernen oder deaktivieren Sie unbenutzte generierte Default-Threads und deren Anlage; keine zusätzliche hochpriore Endlosschleife darf mitlaufen.
5. Binden Sie `lab03_tank.h` im erhaltenen Include-Bereich der erzeugten RTOS-Initialisierung ein. Nach dem einen vorhandenen `osKernelInitialize()`, im Kernelzustand `osKernelReady`, und **vor** dem einen vorhandenen `osKernelStart()` rufen Sie einmal auf:

```c
Lab03_TankStart();
```

   Verwenden Sie einen erhaltenen Benutzerbereich bzw. eine eigene von dort aufgerufene Initialisierungsfunktion. Doppeln Sie Kernelinitialisierung oder -start nicht. Eine unerwartete Rückkehr aus dem Kernelstart gehört in die Fehlerbehandlung.
6. Das Modul erzeugt **TankAlarm** mit `osPriorityAboveNormal` und **TankSampler** mit `osPriorityNormal`, jeweils mit CMSIS-Stackattribut 1024 Byte. Dies sind Ausgangsgrößen, keine nachgewiesenen Minima. Die tatsächliche Heap-/RAM-Bilanz muss auch Idle, optionale Timer-Task und Verwaltungsanteile enthalten. `osThreadNew`-Fehler werden im Modul geprüft. Das Alarmhandle ist vor dem Kernelstart gültig und bleibt während des Betriebs unverändert.
7. Bauen und starten Sie beide Images vor dem Termin. Die unveränderten studentischen Rümpfe warten nur: LD1 bleibt aus, beide Threads müssen ihre Eintrittsfunktionen erreichen und zwischenzeitlich Blocked sein. Dieser Zustand prüft die Basis, noch nicht die geforderte Anwendung. Prüfen Sie erneute CubeMX-Generierung und Build auf den Erhalt der Einbindung. Geben Sie diesen Stand als Projektbasis aus.

## Bearbeitung durch die Studierenden

Die Studierenden ersetzen beide Rümpfe in `tank_threads.c`. Sie schreiben Messschleife, Grenzübergangserkennung, Flagübergabe und die blockierende LED-Pulsfolge selbst. Die anfängliche Hilfsfunktion `WaitForImplementation` wird danach nicht mehr benötigt und kann gelöscht werden. Vollständige Musterfunktionen stehen im `solution`-Abschnitt des Übungsblatts.

`Lab03_TankRead` liefert die zyklische Folge 25, 45, 85, 90, 55, 30 Prozent. Der Mess-Thread ist der einzige Aufrufer. `Lab03_TankWaitNext` erhöht das vorherige Sollziel um 200 Ticks; bei verpasstem Ziel zählt es die Zielverfehlung und richtet auf jetzt plus 200 neu aus. Ein während des API-Übergangs abgelaufenes Ziel wird einmal neu versucht. Weitere Fehler führen zum Fehlerpfad. Die Überlaufbehandlung setzt relevante Zeitabstände unter der halben 32-Bit-Spanne voraus; Zielverfehlungen sind ohne weitere Definition keine allgemeinen Deadline-Miss-Zahlen.

Nur der Signal-Thread schreibt die LED. Der Mess-Thread hält seinen Vergleichszustand lokal; Thread-Flags übertragen einen ausstehenden Reaktionswunsch. Ein Flag zählt beliebig schnelle Grenzübertritte nicht verlustfrei. Die Diagnosewerte haben jeweils einen schreibenden Thread und werden nur bei angehaltenem CM4 gemeinsam gelesen; `volatile` ist keine allgemeine Datensynchronisation.

Der Aufgabenpool ist bewusst nicht auf eine kurze Vorführung beschränkt. Die Lehrperson kann Aufgaben auswählen. Ohne funktionierende Hardware bleiben die Papieraufgaben lösbar; ein nicht ausgeführter Boardtest wird entsprechend benannt. Die Quelldateien ersetzen kein generiertes Firmwareprojekt und sind kein behaupteter Hardwaretest.

## Weiterführender Vergleich

Der ältere Baustein `lab03_rtos.c/.h` mit Heartbeat, HeavyMath und Tasterereignis bleibt für einen getrennten Zusatzversuch verfügbar. Seine GPIO-/EXTI-Integration und alten Rollen sind keine Voraussetzung der neuen Füllstandsaufgabe. Für das Verständnis der Aufgabe 6 werden ausschließlich Threadanlage, Prioritäten, Ticks, blockierendes Warten und Thread-Flags aus Vorlesung 3 benötigt.
