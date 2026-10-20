# Programmieraufgabe zu Übungsblatt 3

Aufgabe 6 verwendet nur **zwei FreeRTOS-/CMSIS-RTOS2-Anwendungsthreads mit LEDs** auf **NUCLEO-H755ZI-Q / STM32H755ZIT6**:

- **Heartbeat / Normal:** LD2/PE1 umschalten und mit `osDelay(500)` blockieren.
- **HeavyMath / BelowNormal:** dauerhaft rechnen und nach jedem Rechenblock LD1/PB0 umschalten.

Ausschließlich **CM4** läuft und übernimmt auch Systemstart und Taktinitialisierung; CM7 bleibt boot-deaktiviert. Die eingebauten LEDs und USB/ST-LINK genügen. B1, EXTI, Thread-Flags und zusätzliche Peripherietimer werden nicht verwendet. Der für FreeRTOS nötige SysTick bedient hier gleichzeitig die HAL-Zeitbasis.

| Datei | Aufgabe |
|---|---|
| `lab03_rtos.c/.h` | genau zwei Threads anlegen, GPIO-Hilfen, Fehlerpfad und Diagnosefelder |
| `lab03_systick.c` | vorbereiteter gemeinsamer 1-kHz-SysTick für HAL und FreeRTOS |
| `threads_student.c` | zwei anfänglich nur blockierende Rümpfe; beide Schleifen selbst implementieren |

Die [Projektvorbereitung](../../labs/vertiefung/lab_03_programmierung.md) beschreibt CubeMX, VS Code mit STM32 VS Code Extension, CM4-Boot-Optionen, eigenständigen Systemstart und die Einbindung. Für die gemeinsame Zeitbasis sind insbesondere `USE_CUSTOM_SYSTICK_HANDLER_IMPLEMENTATION=1` am gesamten CM4-Ziel, genau eine SysTick-Handlerdefinition und die unveränderte SVC-/PendSV-Anbindung erforderlich. Die HAL verwendet SysTick statt TIM6; Tickless Idle ist aus. Diese Konfiguration wird vor dem Termin vorbereitet.

Nach der Kernelinitialisierung und vor dem Kernelstart einmal `Lab03_Start()` aufrufen. Keine zusätzlichen Kopien der Threads generieren. Die früheren Füllstandsdateien `lab03_tank.c/.h` und `tank_threads.c` werden nicht eingebunden.

Aufgaben und vollständige Musterfunktionen stehen im [Übungsblatt](../../labs/lab_03_einfuehrung-freertos-praeemption.qmd), Lösungen ausschließlich im Profil `solution`. Pro Eintrittsfunktion genau eine Definition bauen. Der Ausgangsstand rechnet und blinkt noch nicht. Der Prioritätsversuch ist optional.

Das Repository liefert Anwendungsbausteine und studentische Startdateien, kein generiertes Firmwaregesamtprojekt. Ein Hosttest ersetzt weder einen ARM-Build noch den Boardlauf. Beobachtungen sind keine sicheren WCET- oder Fristnachweise.
