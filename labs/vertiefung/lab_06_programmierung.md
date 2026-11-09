# Projektvorbereitung zu Übungsblatt 6

Aufgabe 6 ist eine **selbst implementierte Polling-Server-Simulation** mit zwei gewöhnlichen FreeRTOS-Threads auf **CM4**. Es wird kein Scheduling-Server in den FreeRTOS-Kernel eingebaut. Datenstruktur und Regeln stehen im [Übungsblatt](../lab_06_server-verfahren.qmd); vollständige Musterfunktionen erscheinen dort ausschließlich im Lösungsprofil. Die Studierenden implementieren die Anwendungslogik selbst. Zusätzliche Laborquelldateien sind nicht erforderlich.

## Projektbasis und CubeMX

Verwenden Sie **NUCLEO-H755ZI-Q / STM32H755ZIT6**, **STM32CubeMX** und **VS Code mit STM32 VS Code Extension**. Die [M4-only-Projektbasis aus Übung 3](lab_03_programmierung.md) ist vor dem Termin auf eigenständigen Start, Taktinitialisierung und gemeinsamen SysTick für HAL und Kernel zu prüfen. Beide Anwendungsthreads laufen im lokalen FreeRTOS-Kernel auf CM4; CM7 bleibt boot-deaktiviert. Der Anwendungscode setzt **C11 oder neuer** voraus; halten Sie Compiler-, CubeMX-, STM32CubeH7-, FreeRTOS- und CMSIS-Adapterversion sowie die tatsächlichen Buildoptionen der ausgegebenen Basis fest.

1. Öffnen Sie die `.ioc`-Datei im CM4-Kontext. **PE1 / LD2** und **PB0 / LD1** als `GPIO_Output` dem CM4 zuordnen. Unter **System Core > GPIO**: Output Push Pull, No pull-up and no pull-down, Low speed, Ausgangspegel Low. Pinbelegung und aktive Polarität gegen UM2408 prüfen. B1/PC13 bleibt unbenutzt.
2. **Middleware and Software Packs > FREERTOS**: Interface **CMSIS_V2**, Präemption an, **1000 Hz**, **32-Bit-Ticks**, Tickless Idle aus. Die HAL verwendet den vorbereiteten gemeinsamen SysTick. Keinen zusätzlichen Zeitbasis-Timer starten. Kontrollieren Sie die tatsächlich erzeugte `FreeRTOSConfig.h`; Menünamen können versionsabhängig abweichen.
3. Unter **Tasks and Queues** Default-Thread entfernen und genau zwei Anwendungsthreads dynamisch anlegen:

| Task | Entry Function | Priorität | Stack in Byte |
|---|---|---|---:|
| Heartbeat | `StartHeartbeatTask` | `osPriorityNormal` | 512 |
| Simulation | `StartSimulationTask` | `osPriorityBelowNormal` | 1024 |

   Bei einer Dialogeinheit von 32-Bit-Wörtern entsprechen die Werte **128** bzw. **256** Wörtern. Kontrollieren Sie die CMSIS-Attribute `.stack_size` in Byte. Die Größen sind Ausgangswerte; Heap/RAM müssen auch Idle, Verwaltungsdaten und Handlerstack berücksichtigen.
4. Unter **Project Manager > Project** CMake wählen, unter **Code Generator** *Keep User Code when re-generating* aktivieren. Nur das CM4-Ziel bauen und laden. Prüfen Sie den Aufruf der GPIO-Initialisierung und den Erhalt der M4-/SysTick-Anpassungen nach erneuter Generierung.

## Was die Studierenden schreiben

Der Typ `Modell` und die selbst geschriebene Funktion `ModellSchritt` stehen im erhaltenen gemeinsamen Benutzerbereich der erzeugten CM4-Anwendungsdatei. Falls Threadfunktionen in einer anderen Datei liegen, werden Typ und Funktionsdeklaration in einem eigenen erhaltenen Deklarationsbereich gemeinsam bekannt gemacht. Es werden keine zweiten Definitionen der vom Generator erzeugten Threadfunktionen hinzugefügt.

Die zwei Schleifen stehen jeweils in ihren `USER CODE`-Bereichen. Heartbeat besitzt ausschließlich LD2. Simulation besitzt ausschließlich LD1 und ihr lokales Modell. Benötigt werden `main.h`, `cmsis_os2.h` und `stdint.h`, für `NULL` außerdem `stddef.h`. Die vom Generator angelegten Handles werden unmittelbar nach beiden `osThreadNew`-Aufrufen und vor `osKernelStart()` geprüft. Bei den vorgeschlagenen Tasknamen heißen sie üblicherweise `HeartbeatHandle` und `SimulationHandle`; prüfen Sie die tatsächlich erzeugten Namen. Es bleiben genau eine Kernelinitialisierung, die zwei generierten Threadanlagen und ein Kernelstart bestehen. Delay- und Anlagefehler führen zu `Error_Handler()` und dürfen danach nicht zur regulären Ausführung zurückkehren. Liegt die Threadanlage in einer anderen Datei, muss ihr Fehlerpfad auch dort verfügbar sein.

Jeder Modellschritt bedeutet **eine virtuelle Millisekunde**. Die Pause von 100 realen Kernelticks verlangsamt die Anzeige. Die Ankünfte bei 1, 3, 7 und 8, das Budget zwei und die Modellperiode sechs sind reine Lehrdaten. Leiten Sie vor dem Boardlauf die Bedienungsschritte und den Endzustand nach Schritt 17 selbst her. Anschließend beginnt das nächste endliche Modell von vorn. Die geänderte erste Ankunft aus Teilaufgabe 3 wird nur auf Papier untersucht; die Firmware behält die ursprüngliche Folge.

## Abnahme und Grenzen

Vor dem Termin müssen CM4-Build, Kaltstart und die beiden Eintrittsfunktionen auf der tatsächlichen Hardware geprüft sein. Erst nach studentischer Implementierung wird das erwartete Modellmuster verlangt. Der Heartbeat soll dabei weiterhin blinken. Ein Hosttest kann die diskrete Modellfolge prüfen, aber weder reale LED-Zeiten noch eine Kernelreservation nachweisen.

Die Simulation reserviert keine echte CPU-Zeit. Insbesondere ist `osDelay(100)` keine Budgetabrechnung; es ist nur die Anzeigepause. Ein vollständiger Sporadic Server würde zusätzlich korrekte Aktivierungs-/Auffüllregeln, tatsächliche CPU-Verbrauchserfassung und die Einbindung in die Schedulingentscheidung benötigen. Der Praxisteil vertieft deshalb die Modellbegriffe, während die Klausuraufgaben die Serveranalyse prüfen.

Board und USB/ST-LINK genügen; zusätzliche Elektronik oder Tracegeräte sind nicht erforderlich. Ein nicht durchgeführter Boardlauf wird als solcher dokumentiert.
