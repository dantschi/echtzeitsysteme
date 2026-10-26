# Implementierungsaufgabe zu Übungsblatt 5

`lab05_button.c/.h` und `button_student.c` bilden die aktuelle Aufgabe: eigener ISR-Try-Put und genau ein blockierender LED-Worker für B1. Die Wertqueue hat drei Plätze. Der vorhandene HAL-Callback leitet seine Pinmaske mit `Lab05_ButtonIRQ(pin)` weiter; `Lab05_ButtonStart()` richtet Queue und Worker einmalig ein.

Der Worker verwendet die erfassten Ereigniszeitpunkte für die 40-Tick-Entprellregel. Für eine gewertete Betätigung schaltet er LD1 60 Ticks ein und anschließend 100 Ticks aus. FreeRTOS/CMSIS-RTOS2 läuft dafür mit 1000 Ticks/s. Die Regel kann bereits verworfene Queue-Nachrichten nicht wiederherstellen und garantiert nicht genau einen Puls je physischem Tastendruck.

[Projektvorbereitung](../../labs/vertiefung/lab_05_programmierung.md) · [Aufgaben und Musterlösung](../../labs/lab_05_deferred-interrupt-processing.qmd)

Die studentischen Startdateien enthalten bewusst noch keine Lösung. Vollständige Musterfunktionen stehen im Quarto-Profil `solution`. Benötigt werden NUCLEO-H755ZI-Q, STM32CubeMX und VS Code mit STM32 VS Code Extension; zusätzliche Elektronik oder Messgeräte sind nicht erforderlich. Die Dateien ergänzen ein vorbereitetes generiertes Projekt, sie ersetzen weder dessen Startup-/Linker-/Kernelkonfiguration noch einen tatsächlichen Boardtest.

## Getrennter Zusatzversuch

`lab05_messages.c/.h` ist der ältere Demonstrator mit Producer, Consumer und vier Queueplätzen. Verwenden Sie ihn nur in einer getrennten Projektkopie; er ist keine zusätzliche Quelle der aktuellen Übungsaufgabe.
