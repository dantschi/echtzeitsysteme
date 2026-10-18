# Projektvorbereitung zu Übungsblatt 2

Die Studierenden implementieren in Aufgabe 6 eine eigene kooperative Signalfolge. Diese Anleitung beschreibt die vorher einzurichtende Projektbasis; sie ist keine zusätzliche Aufgabe auf dem Übungsblatt. Der bisherige A-/B-/C-Demonstrator bleibt als separates Vergleichsbeispiel verfügbar.

## Basis vor dem Termin bereitstellen

1. Verwenden Sie **STM32CubeMX**, **VS Code mit STM32 VS Code Extension** und **NUCLEO-H755ZI-Q / STM32H755ZIT6**. Erzeugen bzw. übernehmen Sie ein Dual-Core-CMake-Projekt mit getrennten CM7-/CM4-Zielen. Die installierte Werkzeugkombination muss diese Erzeugung unterstützen. Erhalten Sie die generierte Boot-/HSEM-Startfolge; CM7 behält Systemstart und gemeinsame Taktkonfiguration, CM4 die Anwendung. Laden Sie die zusammengehörigen Images.
2. Verwenden Sie auf CM4 **kein RTOS**. Entfernen Sie frühere Anwendungsmodule und deren Aufrufe aus dem aktiven Ziel. Behalten Sie eine funktionierende HAL-Zeitbasis mit 1 kHz bei. Prüfen Sie sowohl `HAL_GetTickFreq() == HAL_TICK_FREQ_1KHZ` als auch tatsächlich fortschreitende Tickwerte.
3. Ordnen Sie zwei LEDs ausschließlich CM4 zu: **PB0 / LD1** als `LAB_MARKER`, **PE1 / LD2** als `LAB_STATUS`, jeweils Ausgang, anfangs LOW. Prüfen Sie die Belegung und aktive Polarität anhand der tatsächlichen Boardrevision und UM2408. Die Namen müssen in CM4 `main.h` als Port-/Pin-Makros erscheinen. CM7 darf diese Pins weder konfigurieren noch schreiben.
4. Nehmen Sie `lab02_demo.c`, `lab02_signal.c` und die studentische Startdatei `signal_step.c` aus `code/lab02/` sowie deren Include-Verzeichnis in das **CM4-CMake-Ziel** auf. C11 oder neuer verwenden. Keine zweite Kopie von `Lab02_SignalStep` anlegen. Die neue Anwendung verwendet `Lab02_InitSteps`; die alten A-/B-/C-Schritte werden dabei nicht ausgewählt.
5. Binden Sie `lab02_signal.h` im erhaltenen Include-Bereich der generierten CM4-`main.c` ein. Nach Boot-, HAL- und GPIO-Initialisierung im vorgesehenen USER-CODE-Bereich:

```c
if (!Lab02_SignalInit()) {
    Error_Handler();
    for (;;) {}
}
```

6. Innerhalb der **vorhandenen** `while (1)` steht im erhaltenen Benutzerbereich nur der Dispatcheraufruf:

```c
Lab02_DispatchOnce();
```

   Weder `Lab02_Init` noch `Lab02_SignalInit` zusätzlich in der Schleife aufrufen. Kein eigener SysTick-/PendSV-Handler und keine weitere Bootfolge werden ergänzt.
7. Bauen und starten Sie beide Images vor dem Termin. Mit dem unveränderten studentischen Rumpf bleibt LD1 aus, LD2 schaltet ungefähr alle 700 Tickwerte um und `lab02_signal_checks` steigt. Prüfen Sie im Debugger, dass CM4 die studentische Funktion erreicht. Prüfen Sie einmal erneute CubeMX-Generierung, erneuten Build und den Erhalt der CMake-/USER-CODE-Einbindung. Geben Sie diesen Stand als Projektbasis aus.

## Bearbeitung durch die Studierenden

Die Studierenden ersetzen den leeren Rumpf von `Lab02_SignalStep` durch ihre eigene Zustands-/LED-/Wartelogik und testen beide Signalvarianten. Das Arbeitsziel ist eine selbst implementierte Anwendung, kein bloßer Aufruf des vollständigen alten Demonstrators. Die Musterimplementierung steht ausschließlich im `solution`-Abschnitt des Übungsblatts.

`Lab02_SignalPause(ms)` setzt nur die Bereitschaftsdaten des gerade laufenden Schritts. Der Schritt muss selbst zurückkehren. Der überlauffeste Zeitvergleich setzt positive Warteintervalle und unbeobachtete Verspätungen unter der halben 32-Bit-Spanne voraus. Der Zählschritt bleibt bereit; der Status-Schritt wartet nach seiner LED-Aktion. Diagnosezähler sind bei angehaltenem CM4 zu lesen und laufen modulo 2³².

Die Papieraufgaben bleiben unabhängig vom Board lösbar. Der vollständige Aufgabenpool darf länger als der verfügbare Übungsblock sein; die Lehrperson wählt Aufgaben aus. Ein festes 15-Minuten-Demonstrationsformat gilt für dieses Blatt nicht.

## Weiterführender Vergleich

Der bisherige vollständige Demonstrator mit `LAB02_SHORT`, `LAB02_LONG_B`, `LAB02_VARIABLE_B` und `LAB02_SLEEP_A` ist im [Code-README](../../code/lab02/README.md#bisheriger-aufrufring-als-zusatzversuch) dokumentiert. Verwenden Sie ihn in einem getrennten Projektstand; nicht beide Initialisierungen kombinieren. Seine Iterationszahlen sind keine bekannten Millisekundenzeiten und bilden die neuen Papier-Tasksets nicht nach.

Ein Oszilloskop ist für Aufgabe 6 nicht erforderlich. Bei eigenen Messungen sind Pulsbreite, Startabstand, Tickphase und mögliche Präemption durch Interrupts zu unterscheiden. Ein endliches Messmaximum beweist keine WCET. Dieses Repository enthält Anwendungsbausteine und Startdateien, kein generiertes oder auf dem Board geprüftes Firmwaregesamtprojekt.
