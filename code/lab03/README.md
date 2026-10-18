# Programmieraufgabe zu Übungsblatt 3

`lab03_tank.c/.h` ist die vorbereitete Infrastruktur für eine simulierte Füllstandsüberwachung auf **NUCLEO-H755ZI-Q / STM32H755ZIT6**, CM4 mit FreeRTOS und CMSIS-RTOS2. Die Studierenden ersetzen in `tank_threads.c` die beiden anfänglichen Wartefunktionen durch ihre eigene Mess- und Signallogik. Der Ausgangsstand sampelt noch nicht und schaltet keine LED.

Die [Projektvorbereitung](../../labs/vertiefung/lab_03_programmierung.md) beschreibt CubeMX, VS Code mit STM32 VS Code Extension, CM4-CMake-Einbindung und Kernel-/GPIO-Konfiguration. Einmal nach Kernelinitialisierung und vor Kernelstart `Lab03_TankStart()` aufrufen. Nicht zusätzlich zwei gleichnamige Threads generieren. CM7 behält seine generierte System-/Bootfunktion. Die Projektbasis braucht keinen Taster und keinen ADC.

Aufgaben und vollständige Musterfunktionen stehen im [Übungsblatt](../../labs/lab_03_einfuehrung-freertos-praeemption.qmd), Lösungen im Profil `solution`. Kompilieren Sie pro Eintrittsfunktion genau eine Definition. Der alte vollständige Demonstrator `lab03_rtos.c/.h` bleibt separat verfügbar und wird in dieser Aufgabe nicht verwendet.

Das Repository liefert Anwendungsbausteine und studentische Startdateien, kein generiertes Firmwaregesamtprojekt. Ein Hosttest ersetzt weder einen ARM-Build noch den Boardlauf. Beobachtungen sind keine sicheren WCET- oder Fristnachweise.
