# Kooperativer Aufrufring zu Vorlesung 2

Plattform: **NUCLEO-H755ZI-Q / STM32H755ZIT6**, Anwendung auf **Cortex-M4 ohne Betriebssystem**. Cortex-M7 behält den vorbereiteten generierten Boot-/Systemanteil. Entwicklungsweg: **STM32CubeMX und VS Code mit STM32 VS Code Extension**, zwei getrennte CMake-Ziele. `lab02_demo.c` und `lab02_demo.h` sind vollständige Anwendungsbausteine für eine generierte Basis, kein fertiges CubeMX-Projekt und kein behaupteter Hardwarelauf.

## Was der Versuch untersucht

Drei endliche Schritte A, B, C werden kooperativ im Ring aufgerufen. Jeder muss zurückkehren. Nur A erzeugt einen Messpuls; B verändert seine Iterationszahl; C arbeitet kurz. Die Zeitbasis schreibt nur die HAL-Millisekundenzeit fort. Sie wählt keinen anderen Schritt während einer laufenden Funktion. Es gibt keinen PendSV-Port, keinen getrennten Taskstack und keine RMS-/EDF-Implementierung. Die mathematischen Aufgabenmengen der Vorlesung werden hier nicht auf dem Board nachgebildet.

## Vorbereitung vor dem 25-Minuten-Block

1. Übernehmen Sie die funktionierende Dual-Core-Bare-Metal-Basis aus Vorlesung 1. Versionen von CubeMX, STM32CubeH7-Firmwarepaket, VS Code, STM32-Erweiterung und Compiler festhalten. Die installierte Kombination muss CM4-/CM7-CMake-Ziele und eine geprüfte Lade-/Startfolge unterstützen. Versionsabhängige Menüs werden hier nicht pauschal vorgegeben.
2. Erhalten Sie die zum Projekt gehörende Bootfolge: Bei einem üblichen Template aktiviert M4 eine HSEM-Benachrichtigung und wartet in D2-Stop, M7 initialisiert gemeinsame Takte und gibt den Start frei. Bootkennung, beide Images und Debugkonfiguration müssen zusammenpassen. Keine zusätzliche selbst erfundene Bootfolge ergänzen.
3. Entfernen Sie das alte Lab01-Anwendungsmodul aus dem aktiven CM4-Bauziel sowie dessen Aufrufe aus der M4-Hauptschleife, den Timer-LED-Start und dessen zusätzlichen Callback aus Übung 1. Erforderliche HAL-Zeitbasisbehandlung erhalten. **Kein FreeRTOS aktivieren.** M7 darf weder denselben Markerpin schreiben noch dessen Initialisierung ändern.
4. Lehrtaktraten M7 400 MHz und M4 200 MHz am tatsächlichen Clock-/Stromversorgungssetup prüfen. Eine laufende CM4-HAL-Zeitbasis mit **1 kHz** beibehalten: ein Tickwertschritt ist eine Millisekunde. `HAL_GetTickFreq()` muss `HAL_TICK_FREQ_1KHZ` melden und `HAL_GetTick()` tatsächlich fortschreiten. Das Auslesen des Frequenzenums allein beweist keinen realen Takt. SysTick oder die generierte alternative Zeitbasis muss auch während der Schritte und des Idle-Wartens aktiv sein.
5. Einen freien, herausgeführten digitalen Ausgang anhand Boardrevision, Schaltplan und Pinbelegung auswählen. In CubeMX als Push-Pull-Ausgang, anfangs LOW, **CM4** zuordnen und das User Label **`LAB_MARKER`** setzen. Dadurch entstehen `LAB_MARKER_GPIO_Port` und `LAB_MARKER_Pin` in CM4 `main.h`. Es wird kein pauschaler PA0- oder LED-Anschluss behauptet. GPIO-Takt und tatsächlichen Messanschluss prüfen.
6. Beide Images generieren, bauen, laden und gemeinsam starten. Prüfen Sie, dass CM4 GPIO-Initialisierung und Hauptschleife erreicht. Die Erhaltung von `USER CODE`-Bereichen in CubeMX einschalten. Erste Installation und erster Dual-Core-Start sind Vorbereitung, nicht Teil der 25 Minuten Vergleich.

## Anwendung integrieren

`lab02_demo.c` nur in das **tatsächlich verwendete CM4-CMake-Ziel** und `code/lab02` als Include-Pfad aufnehmen. Das generierte CM4-`main.h` muss ebenfalls erreichbar sein. Der Baustein benötigt C11 oder einen neueren C-Sprachstandard; die Beispielprüfungen benutzen C11. Der Header verlangt `CORE_CM4` und lehnt `CORE_CM7` ab. Prüfen Sie nach erneuter Generierung, dass die eigenen Quelldateien noch in diesem Ziel enthalten sind; verwenden Sie dafür eine vom Generator erhaltene eigene CMake-Einbindung, sofern nötig.

Im vorgesehenen Anwender-Include-Bereich der CM4-`main.c`:

```c
#include "lab02_demo.h"
```

Nach generiertem Boot-, HAL- und GPIO-Setup im bestehenden `USER CODE BEGIN 2`:

```c
if (!Lab02_Init(LAB02_SHORT)) {
    Error_Handler();
}
```

Innerhalb der **bereits vorhandenen** `while (1)` im zugehörigen Anwenderbereich:

```c
Lab02_DispatchOnce();
```

Keine zweite `main`, keine weitere Dispatcher-Endlosschleife im Schleifenrumpf, kein Aufruf aus einem Handler oder einem anderen Schritt. Es wird kein SysTick- oder PendSV-Handler ersetzt. `Lab02_Init` ist einmal pro Reset vorgesehen; wiederholte oder ungültige Initialisierung wird abgelehnt.

## Vier Läufe mit gleichem Build- und Taktstand

Ändern Sie nur die Modusauswahl im Initialisierungsaufruf, bauen und starten Sie nach Reset neu:

| Auswahl | B-Arbeit | A-Warteverhalten |
|---|---|---|
| `LAB02_SHORT` | stets kurz | bleibt bereit |
| `LAB02_LONG_B` | stets lang | bleibt bereit |
| `LAB02_VARIABLE_B` | zuerst kurz, dann lang, abwechselnd | bleibt bereit |
| `LAB02_SLEEP_A` | stets kurz | nach Arbeit Wake-Ziel plus 10 Tickwerte |

Die Standardzahlen sind 30000 und 120000 Iterationen. Sie sind **keine bekannten Millisekundenzeiten**. Jede Iteration aktualisiert einen eigenen beobachtbaren 32-Bit-Ergebniswert modulo 2³²; `volatile` verhindert hier das vollständige Entfernen dieser Zugriffe. Die Lehrperson kann vorab `LAB02_SMALL_WORK` und `LAB02_LARGE_WORK` als CM4-Compile-Definitionen anpassen. Kurz muss positiv, lang größer als kurz sein. Danach verwenden alle Vergleichsläufe dieselben Zahlen, Optimierung und Takte.

## Kontrollfluss und Wartevertrag

`Lab02_DispatchOnce` prüft zuerst alle drei Wake-Zeiten und dann höchstens drei Kandidaten ab dem Ringcursor. Nach Auswahl zeigt der Cursor auf den Nachfolger. Genau ein Schritt wird aufgerufen. Der aktive Index gilt nur zwischen Aufruf und Rückkehr; außerhalb steht `LAB02_NONE`. Bereitschaftsfeld und aktiver Aufruf kennzeichnen gemeinsam den Zustand. Die Liste enthält keine eigenen pausierten Stackpointer.

`Lab02_SleepAfterStep(ms)` akzeptiert nur einen Aufruf aus einem aktiven Schritt im Hauptprogramm und eine Wartezahl **0 < ms < 2³¹**. Es setzt `wake_at = HAL_GetTick() + ms` und Blocked. Der aufrufende Schritt muss anschließend **selbst return ausführen**. Eine Rückkehr nur aus der Hilfsfunktion beendet nicht den Schritt. Im Beispiel tut A dies ausdrücklich. Das Wake-Ziel macht A später nur bereit; der tatsächliche nächste Start wartet auf Rückkehr anderer Schritte und passende Ringauswahl. Wegen Tick-Abtastphase bedeutet plus zehn Tickwerte weder eine garantierte genaue physische Startperiode von zehn Millisekunden noch ein periodisches RMS-Release.

`Lab02_TimeReached(now, target)` prüft `(uint32_t)(now - target) < 0x80000000`. Warteintervalle und unbeobachtete Verspätungen müssen strikt unter der halben 32-Bit-Spanne bleiben, hier etwa 24,9 Tage. Der Sonderfall einer Differenz genau 2³¹ liegt außerhalb des Vertrags. `wake_at = 0` ist ein gültiger Zielzeitpunkt und braucht daher ein eigenes Bereitschaftsfeld. Vorzeichenlose Arithmetik behandelt den Zählerüberlauf; ein einfacher Vergleich `now >= target` wäre falsch.

Wenn alle Einträge Blocked sind, zählt der Dispatcher einen Idle-Eintritt und benutzt `__WFI`, Wait For Interrupt. Eine aktive Tick-/Wake-Quelle ist Voraussetzung. Ein bereits vor dem Warten behandelter Tick garantiert keinen sofortigen neuen Test; gegebenenfalls weckt erst der nächste Tick. Im unveränderten Demonstrator bleiben B und C bereit, der vollständige Idle-Fall wird in der Hostprüfung gezielt gesetzt.

`__get_IPSR()` liest das aktuelle Ausnahmekennzeichen des Cortex-M: null bedeutet hier Hauptprogramm, ein anderer Wert Handlerkontext. Die Anwendung lehnt dort Setup/Schlafaufrufe ab und behandelt einen Dispatcheraufruf als Fehler. Rekursiver Dispatch und ein ungültiger Bereitschaftszustand führen ebenfalls zum Fehlerpfad. `Error_Handler()` wird aufgerufen; falls er unerwartet zurückkehrt, hält eine Schleife den Anwendungsfortschritt an. Dies ist eine Diagnose, keine implementierte funktionale Sicherheitsreaktion.

## Beobachtung und Abgabe

`lab02_diag` enthält Aufrufzähler, Ergebniswerte, Idle-Eintritte und Fehlerzähler. Bei gültigen Aufrufen schreibt nur das Hauptprogramm mit seinen Schritten die Diagnose- und Verwaltungsdaten; Interrupts schreiben die HAL-Zeitbasis. Der Fehlerzähler kann auch im abgewiesenen Handlerkontext gesetzt werden, bevor der Fehlerpfad den Fortschritt anhält; er garantiert keine vollständige Zählung beliebig verschachtelter Fehler. Die Diagnosezähler laufen modulo 2³². Einen konsistenten Gesamtstand lesen Sie nur bei angehaltenem CM4; dieser Halt gehört nicht zu einer Timing-Messreihe. `volatile` ersetzt keine allgemeine Datenübergabe zwischen Kernen.

Ein Oszilloskop/Logikanalysator misst den gewählten A-Marker mit geprüftem Anschluss und gemeinsamem Massebezug. Pulsbreite und Abstand steigender Flanken unterscheiden. Der Puls enthält die A-Rechnung, Teile der GPIO-Aufrufe und mögliche Interruptarbeit, nicht die gesamte Dispatcherfunktion. Mindestens 20 Pulse pro verglichener Variante sowie Trigger, Zeitauflösung, Build, Takte und Lastauswahl dokumentieren. Ein Messmaximum ist keine sichere WCET-Schranke. Modellwerte der Papieraufgabe gehören nicht in die Messspalten.

Ohne Messgerät bleiben Hardwarezeiten leer. Diagnosezähler, Codelektüre und begründete Prognose sind dann zulässig; ein fehlender Aufbau wird als **nicht durchgeführt** markiert. Eine absichtlich nie zurückkehrende Funktion wird nur auf Papier besprochen, nicht als verpflichtender Boardversuch ausgeführt.

## Quellen und Prüfstatus

Theorie: Buttazzo, *Hard Real-Time Computing Systems*; Liu und Layland (1973). Board/Peripherie: ST UM2408 und RM0399, Cortex-M: PM0214 und CMSIS-Core. Werkzeugweg: [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html), [STM32 VS Code Extension](https://www.st.com/en/development-tools/stm32-vscode.html).

Ausgeführt wurden unabhängige Ereignissimulationen der periodischen 75-/85-/97-Prozent-Sets unter RMS und EDF über jeweils zwei Hyperperioden, die Antwortzeititerationen und Schrankenrechnungen sowie der Abgleich jeder gezeigten Millisekunde der vier zugehörigen Scheduling-Diagramme. Hinzu kommen Kapazitätsgegenbeispiele und Ring-/Jitterrechnungen. Diese Modellprüfungen sind keine Boardzeiten.

Der C-Baustein wurde mit C11, `-Wall -Wextra -Werror` gegen die echten ST-HAL-/CMSIS-Header syntaktisch geprüft. Headerstände: ST HAL `7e541d92019e18f98d211fc4ab9197ec8e8105f6`, STM32H7 CMSIS-Device `81db1ec63cdc191fae1565b772da3ea5aa29a683`. Nur die wegen 64-Bit-Hostzeigern entstehenden Zeigerbreitenwarnungen sind für diese API-Prüfung ausgenommen. Das falsche Kernziel wird erwartungsgemäß abgelehnt.

17 Host-Szenarien prüfen vier Modi, die beobachtbare Rechenarbeit, Markerreihenfolge, Wake-Auswahl, All-Blocked-Idle, Ticküberlauf einschließlich Zielwert null, ungültige Warteintervalle, falschen Aufrufkontext, Initialisierungsfehler, Rekursion, ungültigen Zustand und Diagnosezählerüberlauf. Für gezielte Idle-/Fehlerfälle setzt der Test private Zustandsdaten. HAL, Ausnahmekennzeichen und WFI sind dabei modelliert, keine laufende STM32-Peripherie. **ARM-Build mit Startup/Linkerskript, Dual-Core-Boardtest und reale Zeitmessungen wurden nicht durchgeführt.**
