# Programmieraufgabe zu Übungsblatt 3

Aufgabe 6 implementiert das Vorlesungsbeispiel **HeavyMath, Heartbeat und Reaktion auf B1** mit **FreeRTOS / CMSIS-RTOS2** auf **NUCLEO-H755ZI-Q / STM32H755ZIT6**. Ausschließlich **CM4** läuft und übernimmt auch den Systemstart und die Taktinitialisierung. CM7 bleibt boot-deaktiviert; es wird kein CM7-Image benötigt. Die eingebauten LD1/PB0, LD2/PE1 und B1/PC13 genügen.

- `lab03_rtos.c/.h`: vorbereitete Threadanlage, GPIO-Hilfen, absolute Heartbeat-Wartehilfe und sichere IRQ-Übergabe. Der frühere vollständige Demonstrator ist damit zur Infrastruktur der Implementierungsaufgabe geworden.
- `threads_student.c`: drei anfänglich nur blockierende Rümpfe. Die Studierenden implementieren **alle drei Threadschleifen selbst**.
- `lab03_tank.c/.h`, `tank_threads.c`: frühere Füllstandsvariante; nicht zusammen mit der aktuellen Aufgabe einbinden.

Die [Projektvorbereitung](../../labs/vertiefung/lab_03_programmierung.md) beschreibt CubeMX, VS Code mit STM32 VS Code Extension, CM4-Boot-Optionen, eigenständigen Systemstart, CMake-Einbindung sowie Kernel-/GPIO-/EXTI-Konfiguration. Nach der Kernelinitialisierung und vor dem Kernelstart einmal `Lab03_Start()` aufrufen. Keine zusätzlichen Kopien der Threads generieren. Den CM4-HAL-EXTI-Callback genau einmal an `Lab03_ButtonIRQ()` weiterleiten und den IRQ zunächst gesperrt lassen.

Aufgaben und vollständige Musterfunktionen stehen im [Übungsblatt](../../labs/lab_03_einfuehrung-freertos-praeemption.qmd), Lösungen ausschließlich im Profil `solution`. Pro Eintrittsfunktion genau eine Definition bauen. Der Ausgangsstand rechnet und blinkt noch nicht; erst die eigene Implementierung erzeugt die geforderte Anwendung. Für den Prioritätsversuch nur `math_attr.priority` in `Lab03_Start()` ändern und danach wiederherstellen.

Das Repository liefert Anwendungsbausteine und studentische Startdateien, kein generiertes Firmwaregesamtprojekt. Ein Hosttest ersetzt weder einen ARM-Build noch den Boardlauf. Beobachtungen sind keine sicheren WCET- oder Fristnachweise.
