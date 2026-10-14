# Freiwillige Programmiervertiefung zu Übungsblatt 2

Diese Anleitung ist für die **Vorbereitung durch die Lehrperson** oder eine **freiwillige Vertiefung außerhalb des Übungstermins** gedacht. Sie gehört nicht zum verbindlichen Papierkern. Die Zeitangaben der folgenden Abschnitte gelten allenfalls für einen bereits vollständig vorbereiteten, funktionierenden Projektstand; sie sind keine Zusage für Erstintegration und Fehlersuche.

Verwenden Sie STM32CubeMX und VS Code mit STM32 VS Code Extension für das NUCLEO-H755ZI-Q / STM32H755ZIT6. Die Quellen in `code/lab02/` sind Anwendungsbausteine; ein vollständig erzeugtes, auf dem Board geprüftes Firmwareprojekt wird hier nicht bereitgestellt. Versionen, Startfolge und tatsächliche Integration müssen vor der Demonstration geprüft werden.

Im Übungstermin wird höchstens eine kurze vorbereitete Demonstration gezeigt. Bei einem technischen Problem wechseln Sie spätestens nach drei Minuten zum vollständig angegebenen Papierfall im [Übungsblatt](../lab_02_scheduling-theorie-kooperativer-scheduler.qmd). Studierende müssen frühere Programmiervertiefungen nicht erfolgreich abgeschlossen haben, um dem nächsten Übungsblatt folgen zu können.

Wer freiwillig selbst integriert, dokumentiert nur den tatsächlich erreichten Stand. Modellwerte werden nicht als Boardmesswerte eingetragen. Die folgenden technischen Erläuterungen sind offen lesbar und unterstützen die Vorbereitung; die klausurnahen Musterlösungen bleiben im Profil `solution` des Übungsblatts.

## Teil B: Vorbereiteter Programmvergleich · 25 Minuten

### Aufbau kontrollieren · 5 Minuten

1. Die Dual-Core-Bare-Metal-Basis aus Vorlesung 1 ist bereits generiert, gebaut, geladen und korrekt gestartet. CM4 bezeichnet das Cortex-M4-Firmwareziel, CM7 das Cortex-M7-Ziel. Die generierte Bootfolge bleibt erhalten.
2. Prüfen Sie die tatsächlichen Takte und die fortschreitende **HAL-Zeitbasis mit 1 kHz** auf CM4. HAL bedeutet Hardware Abstraction Layer. `HAL_GetTickFreq()` meldet `HAL_TICK_FREQ_1KHZ`, `HAL_GetTick()` muss aber auch wirklich fortschreiten. Der Tickinterrupt wählt keinen anderen Schritt aus. Kein FreeRTOS aktivieren.
3. Entfernen Sie das alte Lab01-Modul aus dem CM4-Bauziel und dessen Timer-/LED-Aufrufe aus Übung 1, ohne die erforderliche Zeitbasisbehandlung zu löschen. Wählen Sie anhand Schaltplan und Boardbelegung einen freien herausgeführten GPIO-Ausgang, nur CM4 zugeordnet. GPIO heißt General-Purpose Input/Output. In CubeMX erhält er das **User Label `LAB_MARKER`**, Ausgangsmodus und Anfangspegel LOW. Port-/Pinmakros in CM4 `main.h` und den tatsächlichen Messanschluss prüfen. PA0 ist keine pauschale Vorgabe.
4. Werkzeugstände, Compiler-/Optimierungsoptionen und die CM4-/CM7-CMake-, Flash- und Debugkonfiguration dokumentieren. Die Quelle `lab02_demo.c` sowie deren Header sind bereits nur in CM4 eingebunden; erneute Generierung darf diese Einbindung nicht verlieren.

Im vorgesehenen Anwender-Include-Bereich von CM4 `main.c` steht `#include "lab02_demo.h"`. Nach erfolgreichem generierten HAL-/GPIO-Setup:

```c
if (!Lab02_Init(LAB02_SHORT)) {
    Error_Handler();
}
```

Im Rumpf der **vorhandenen** `while (1)` steht:

```c
Lab02_DispatchOnce();
```

Keine zusätzliche Endlosschleife, kein Step-Aufruf aus einem anderen Step und kein eigener PendSV- oder SysTick-Handler. Die vollständige Vorbereitung steht im Modul-README.

### Vier Läufe lesen und vergleichen · 15 Minuten

Ändern Sie nur die Modusauswahl in `Lab02_Init`, bauen und starten Sie nach Reset neu. Die vorbereitete Basis und die Iterationszahlen bleiben gleich.

| Auswahl | Was verändert wird |
|---|---|
| `LAB02_SHORT` | A, B und C bleiben mit kurzer Arbeit bereit |
| `LAB02_LONG_B` | B hat konstant mehr Iterationen |
| `LAB02_VARIABLE_B` | B wechselt zwischen kurzer und langer Arbeit |
| `LAB02_SLEEP_A` | A setzt nach Arbeit ein Wake-Ziel plus zehn Tickwerte und kehrt zurück |

Die Standardzahlen 30000 beziehungsweise 120000 Iterationen sind **keine zugesicherten Millisekundenzeiten**. Diagnosefelder `lab02_diag.runs`, `result`, `idle_entries` und `failures` sind nur für einen bei angehaltenem CM4 konsistent gelesenen Zustand gedacht. Ein Debughalt gehört nicht in eine Zeitmessreihe.

Bearbeiten Sie dazu:

- Warum hat jeder ständig bereite Schritt eine Ringposition, aber keine fest zugesagte Zeitscheibe?
- Welcher Teil des Kontrollflusses verhindert einen A-Aufruf während laufender B-Arbeit, obwohl Tickinterrupts die Zeit fortschreiben?
- Was speichert der TCB, Task Control Block oder Verwaltungsdatensatz? Warum ist `running_idx` außerhalb eines Schrittaufrufs ungültig?
- Warum muss A nach erfolgreichem `Lab02_SleepAfterStep(10U)` selbst zurückkehren? Weshalb sind zehn Tickwerte keine genaue physische Startperiode?
- Warum prüft der Dispatcher höchstens drei Kandidaten? Was tut er bei drei Blocked-Einträgen? Lesen Sie dazu den Idle-Pfad mit `__WFI`, Wait For Interrupt; eine aktive Wake-Quelle ist erforderlich.
- Warum reicht `now >= wake_at` beim Überlauf nicht? Der Modulovergleich ist nur bei Warteintervallen und unbeobachteten Verspätungen strikt unter $2^{31}$ Millisekunden gültig.

Eine nie zurückkehrende B-Funktion wird **auf Papier** besprochen, nicht als verpflichtender Boardversuch ausgeführt. Zum Vergleich genügt die begrenzte längere Last.


### Technische Erläuterung des Programmvergleichs

Der Ringcursor legt den Suchbeginn fest und rückt nach einer Auswahl auf den Nachfolger. Bei endlichen Schritten erhalten ständig bereite Teilnehmer wiederholt Aufrufe, aber unterschiedlich lange Schritte behalten den Kern unterschiedlich lange. Die Tickroutine aktualisiert nur die Zeit; sie tauscht im Demonstrator keine Taskkontexte. Daher kann A erst nach Rückkehr aus B und späterer Auswahl erneut laufen.

Der TCB enthält Funktionszeiger, Bereitschaftsfeld und Wake-Zeit. Es gibt keinen pausierten Stack pro Schritt. `running_idx` bezeichnet nur zwischen Funktionsaufruf und Rückkehr den aktiven Eintrag; außerhalb steht `LAB02_NONE`. Die Schlafhilfe setzt Verwaltungsdaten, beendet aber nicht den aufrufenden Schritt. A tut dies selbst mit `return`. Relative Warteziele nach Arbeit, Tick-Abtastphase und Ringauswahl erzeugen keinen festen Startplan bei 0, 10, 20 ms.

Die Suche hat höchstens drei Kandidaten, statt endlos nach einem bereiten Eintrag zu suchen. Im All-Blocked-Fall zählt sie Idle und wartet mit `__WFI` auf eine zugelassene Quelle. Ein gerade vorher behandelter Tick kann die nächste Prüfung erst durch den folgenden Tick ermöglichen. Im unveränderten Demonstrator bleiben B und C bereit; der vollständige Idle-Pfad ist trotzdem implementiert.

Vorzeichenlose Differenzbildung modulo $2^{32}$ unterstützt den Überlauf. Bei `now = 0xFFFFFFF0` und `target = 0x00000010` ist das Ziel noch 32 Tickwerte entfernt, also nicht erreicht. Bei `now = 0x00000020` ist es um 16 Tickwerte überschritten, also erreicht. Die halbe Spanne begrenzt die eindeutige Ordnung. Ein Zielwert null ist zulässig; deshalb wird die Wartebedingung durch ein eigenes Zustandssymbol markiert. Der konkrete Code liegt offen im Modul und wird nicht als eigener präemptiver RMS-/EDF-Scheduler ausgegeben.

### Protokoll und Grenzen · 5 Minuten

Mit geeignetem Oszilloskop/Logikanalysator können beispielsweise 20 A-Pulse je verglichener Variante erfasst werden. Prüfen Sie den tatsächlichen Pinanschluss und gemeinsamen Massebezug. Die Pulsbreite umfasst A-Arbeit, Teile der GPIO-Aufrufe und mögliche Interruptunterbrechungen. Der Abstand steigender Flanken erfasst die Wiederaufrufe. Die ganze Dispatcherfunktion wird dadurch nicht markiert.

| Konfiguration | Eigener Eintrag |
|---|---|
| Board / Firmwarepaket / Werkzeugversionen | |
| Compiler und Optimierung | |
| M4- / M7-Takt, Tickquelle und Tickfrequenz | |
| Markerpin und Boardanschluss | |
| Iterationszahlen / Modus / Reset | |
| Trigger / Zeitauflösung / Anzahl | |

| Variante | Anzahl | Pulsbreite min./max. | Startabstand min./max. | Startabstandsspanne | Einheit / Status |
|---|---:|---|---|---|---|
| Kurz konstant | | | | | |
| B lang konstant | | | | | |
| B wechselnd | | | | | |
| A wartet relativ | | | | | |

Die Tabelle bleibt bis zur realen Messung leer. Status beispielsweise **gemessen**, **nur Code/Diagnose betrachtet**, **fehlgeschlagen** oder **nicht durchgeführt**. Ohne Messgerät werden keine Hardwarezeiten eingesetzt. Die Zahlen aus Aufgabe 5 gehören nicht in diese Spalten.


### Technische Erläuterung der Messauswertung

Konstante zusätzliche B-Arbeit vergrößert im idealisierten Ring den A-Startabstand, ohne allein eine Schwankung innerhalb des Laufs zu erzeugen. Wechselnde B-Arbeit kann die Abstände schwanken lassen. Auf dem echten Board können auch konstante Läufe durch Interrupts oder Speicherzugriffe variieren. A-Arbeit selbst bleibt gleich, ihre verstrichene Markerbreite kann dennoch zusätzliche Unterbrechungszeit enthalten.

Die Spanne ist Maximum minus Minimum der **benannten** Messgröße. Ein endliches Maximum der Markerbreite beweist keine WCET, Worst-Case Execution Time oder sichere obere eigene Ausführungszeit. Die Pflichtlast der theoretischen Sets wird im kooperativen Programm nicht nachgebildet; ihre RMS-/EDF-Tests sind dadurch nicht bestätigt. Bei fehlendem Aufbau oder Gerät bleiben die Zeitspalten leer und der tatsächlich erfolgte Prüfstatus wird angegeben.
