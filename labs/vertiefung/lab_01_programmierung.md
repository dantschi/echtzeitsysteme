# Freiwillige Programmiervertiefung zu Übungsblatt 1

Diese Anleitung ist für die **Vorbereitung durch die Lehrperson** oder eine **freiwillige Vertiefung außerhalb des Übungstermins** gedacht. Sie gehört nicht zum verbindlichen Papierkern. Die Zeitangaben der folgenden Abschnitte gelten allenfalls für einen bereits vollständig vorbereiteten, funktionierenden Projektstand; sie sind keine Zusage für Erstintegration und Fehlersuche.

Verwenden Sie STM32CubeMX und VS Code mit STM32 VS Code Extension für das NUCLEO-H755ZI-Q / STM32H755ZIT6. Die Quellen in `code/lab01/` sind Anwendungsbausteine; ein vollständig erzeugtes, auf dem Board geprüftes Firmwareprojekt wird hier nicht bereitgestellt. Versionen, Startfolge und tatsächliche Integration müssen vor der Demonstration geprüft werden.

Im Übungstermin wird höchstens eine kurze vorbereitete Demonstration gezeigt. Bei einem technischen Problem wechseln Sie spätestens nach drei Minuten zum vollständig angegebenen Papierfall im [Übungsblatt](../lab_01_bare-metal-polling-interrupts.qmd). Studierende müssen frühere Programmiervertiefungen nicht erfolgreich abgeschlossen haben, um dem nächsten Übungsblatt folgen zu können.

Wer freiwillig selbst integriert, dokumentiert nur den tatsächlich erreichten Stand. Modellwerte werden nicht als Boardmesswerte eingetragen. Die folgenden technischen Erläuterungen sind offen lesbar und unterstützen die Vorbereitung; die klausurnahen Musterlösungen bleiben im Profil `solution` des Übungsblatts.

## Programmvergleich: vorbereitete 35 Minuten

Entwicklungsweg: **STM32CubeMX**, **VS Code mit STM32 VS Code Extension** und **NUCLEO-H755ZI-Q**. Benutzen Sie das von der Lehrperson vorbereitete Dual-Core-Projekt und die vollständigen Anwendungsbausteine in [code/lab01](../../code/lab01/README.md). Deren README beschreibt Vorbereitung, Integration, Dateien und Prüfgrenzen. Es gibt kein Betriebssystem und keinen eigenen gemeinsamen Datenblock.

### Vorbereitung prüfen · 5 Minuten

- Beide Firmwareziele CM4 und CM7 sind bereits gebaut, geladen und korrekt gestartet. Generierten Bootcode erhalten.
- Nur CM4 besitzt TIM2 und PE1 / gelbe LD2. Pinbelegung/Bestückung am Board prüfen.
- TIM2 hat einen geprüften Updateabstand von 500 ms und eine aktivierte CM4-Interruptkette; TIM2 ist nicht die HAL-Zeitbasis. Der Anwendungsbaustein startet TIM2 nur im Timer-Modus.
- CubeMX-, Firmwarepaket-, Erweiterungs- und Compilerstand sowie Takte festhalten. Bei nicht funktionierendem Aufbau die offene Stelle dokumentieren, keine Messergebnisse einsetzen.

### Anwendung und vier Läufe · 20 Minuten

`lab01_demo.c` und den Include-Pfad nur in das tatsächliche CM4-CMake-Ziel aufnehmen. Im vorhandenen Anwender-Include-Bereich `#include "lab01_demo.h"` ergänzen. Nach generierter GPIO-/TIM2-Initialisierung:

```c
if (!Lab01_Init(LAB01_BASELINE, &htim2)) {
    Error_Handler();
}
```

Im Rumpf der **vorhandenen** Endlosschleife nur einen Anwendungsschritt aufrufen:

```c
Lab01_Step();
```

Den vorhandenen Timer-Callback einmal um `Lab01_TimerCallback(htim);` ergänzen. Vorhandene HAL-Zeitbasisbehandlung erhalten und keinen gleichnamigen Callback doppelt definieren. Die komplette Einbindung steht im README.

Wechseln Sie nacheinander den ersten Initialisierungsparameter zu:

1. `LAB01_BASELINE`: Main-Umschaltung, Delay ohne Zusatzlast.
2. `LAB01_POLL_CONSTANT`: feste Iterationszahl zusätzlich zum Delay.
3. `LAB01_POLL_VARIABLE`: wechselnde Iterationszahl zusätzlich zum Delay.
4. `LAB01_TIMER`: Main nur variable Last; Umschaltung ausschließlich im TIM2-Callback.

Nach jeder Änderung neu bauen und nach Reset korrekt starten. Entfernen Sie anderen LED-/Delay-Code, der den Vergleich überlagern würde. Eine Zähliteration hat keine hier bekannte Dauer in Millisekunden. Lesen Sie die Diagnosedaten `lab01_diag` nur bei angehaltenem CM4 als konsistenten Gesamtstand; Debughalt nicht in eine Timing-Aufzeichnung aufnehmen.

### Protokoll und Interpretation · 10 Minuten

Mit geeignetem Messgerät erfassen Sie pro Modus beispielsweise 20 aufeinanderfolgende Toggle-Abstände. Der tatsächliche Messanschluss und gemeinsame Massebezug müssen geprüft sein. Falls nur steigende Flanken erfasst werden, dokumentieren Sie stattdessen vollständige Blinkzyklen und passen die Sollgröße an. Ohne Messgerät ist nur die qualitative LED-Beobachtung zulässig.

| Konfigurationsangabe | Eigener Eintrag |
|---|---|
| Board und Firmwarepaket | |
| CubeMX und STM32-Erweiterung | |
| Compiler / Optimierung | |
| M4- und tatsächlicher TIM2-Takt | |
| PSC / ARR / Interruptkonfiguration | |
| Messpunkte / Trigger / Zeitauflösung | |

| Modus | Anzahl Abstände | Min. | Max. | Mittel | Spanne | Einheit / Status |
|---|---:|---:|---:|---:|---:|---|
| Baseline | | | | | | |
| Polling konstant | | | | | | |
| Polling variabel | | | | | | |
| Timer | | | | | | |

Die Tabelle bleibt bis zur Messung leer. Status beispielsweise „gemessen unter angegebenen Bedingungen“, „nur qualitativ beobachtet“ oder „nicht durchgeführt“. Beantworten Sie:

- Welcher Anteil erklärt einen vergrößerten mittleren Flankenabstand, welcher eine Schwankung?
- Weshalb kann ein Timer-Callback während langer Hauptarbeit reagieren?
- Welche Verzögerungen bleiben möglich, und weshalb beweist der Versuch keine harte Zeitgarantie?


### Technische Erläuterung Programmvergleich

Die Baseline enthält bereits Wartefunktions-, Tick- und Aufrufeffekte. Konstante Zusatzarbeit vergrößert im idealisierten Modell den Flankenabstand; allein daraus folgt noch kein Periodenjitter. Wechselnde Last kann unterschiedliche Abstände verursachen. Welche Spanne tatsächlich vorliegt, muss gemessen werden. Der Timer zählt unabhängig vom Ende der Hauptschleifenarbeit und kann diese bei zulässigem Interrupt unterbrechen. Maskierung, dringendere Handler, Bus-/Speicherlast und die Handlerkette bleiben als Verzögerungen relevant.

Im Timer-Modus zählt `lab01_diag.timer_callbacks` die behandelten Callback-Aufrufe, nicht garantiert alle Hardwareupdates. Bei zu langsamer Bedienung können Ereignisse zusammenfallen. Zwei Toggles bilden einen LED-Zyklus, sodass 500-ms-Updateabstand ideal einen 1-Hz-Blinkzyklus ergibt. Eine optisch stabile LED oder 20 günstige Perioden beweisen weder eine sichere WCET-Schranke noch die maximale Antwortzeit für alle zugelassenen Lasten. Keine beispielhaften Zeitwerte in die leeren Tabellen eintragen. Ein fehlender Aufbau bleibt als „nicht durchgeführt“ markiert.
