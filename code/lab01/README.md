# Bare-Metal-Vergleich zu Vorlesung 1

Plattform: **NUCLEO-H755ZI-Q** mit STM32H755ZIT6. Die Anwendung läuft auf **Cortex-M4 ohne Betriebssystem**; Cortex-M7 behält den vorbereiteten generierten Boot-/Systemanteil. Entwicklungsweg: **STM32CubeMX und VS Code mit STM32 VS Code Extension**, zwei getrennte CMake-Ziele. Diese Dateien sind vollständige Anwendungsbausteine für ein generiertes Projekt, kein fertiges CubeMX-Boardprojekt und kein ausgeführter Hardwareversuch.

## Vorbereitung durch die Lehrperson

Die Installation und der erste Dual-Core-Start sind Vorbereitung, nicht Teil der 35 Minuten Vergleichsversuch.

1. Installieren Sie STM32CubeMX, ein dazu passendes STM32CubeH7-Firmwarepaket, VS Code, die STM32 VS Code Extension und ihre benötigten Build-/Debugwerkzeuge. Halten Sie Versionen und Paketstand fest. Prüfen Sie, dass die verwendete Kombination getrennte CM4-/CM7-CMake-Ziele und deren Debugkonfiguration unterstützt. Versionsabhängige Menübezeichnungen werden hier nicht vorausgesetzt.
2. Verwenden Sie die Boardauswahl **NUCLEO-H755ZI-Q** beziehungsweise ein dafür geprüftes STM32H755ZIT6-Projekt. Bewahren Sie die funktionierende Dual-Core-Bootfolge des generierten Templates. Bei Templates mit M4 in D2-Stop und M7-HSEM-Freigabe müssen beide Images zusammenpassen. Bootkennung und Debug-Startfolge aus dem tatsächlichen Template übernehmen, keine zweite selbstgeschriebene Bootfolge ergänzen.
3. **PE1 / LD2 gelb** als digitalen Push-Pull-Ausgang, anfangs LOW, für **CM4** konfigurieren. Pinzuordnung/Bestückung am verwendeten MB1363-Board und UM2408 prüfen. M7 darf diesen Ausgang weder initialisieren noch toggeln. Kein zusätzlicher Taster oder Messpin wird vorausgesetzt.
4. **TIM2** als aufwärtszählenden Basistimer mit internem Takt und Update-Interrupt für **CM4** einrichten. M7 darf TIM2 nicht benutzen. TIM2 darf nicht die HAL-Zeitbasis sein und nicht anderweitig gestartet werden. Eine funktionierende HAL-Zeitbasis auf CM4 beibehalten, im vorbereiteten Versuch mit 1 kHz (1 ms pro Tick), vorzugsweise SysTick. Dies ist kein aktiver Betriebssystem-Tick. Die HAL-Zeitbasiskonfiguration tatsächlich prüfen. Bei anderer Zeitbasis ihre vorhandene Behandlung erhalten.
5. Die Lehrtaktraten M7 400 MHz und M4 200 MHz sowie die passende Stromversorgungs-/Taktkonfiguration am vorbereiteten Projekt prüfen. Diese Werte sind Annahmen, keine Messung. Den **tatsächlichen TIM2-Kerntakt** im generierten Projekt prüfen: Bus-Vorteiler und Timer-Taktauswahl sind relevant. Kerntakt oder `SystemCoreClock` nicht als Timerfrequenz übernehmen. Nur bei 100 MHz TIM2-Takt liefern Prescaler **9999** und Counter Period / ARR **4999** zwei Updates pro Sekunde. Andernfalls passende Werte für denselben Updateabstand von 500 ms berechnen. Clock-Tree und Werte dokumentieren; keine maximale Datenblattfrequenz als Isttakt ausgeben.
6. Die TIM2-Anforderung in der CM4-Interruptsteuerung NVIC aktivieren und die erzeugte Kette `TIM2_IRQHandler` → `HAL_TIM_IRQHandler(&htim2)` prüfen. NVIC bedeutet Nested Vectored Interrupt Controller. Im Versuch gibt es kein RTOS und damit keine FreeRTOS-IRQ-Schwelle. Keine willkürlich erfundene Prioritätszahl übernehmen; verwendete Prioritätsgruppierung und relevante andere Interrupts dokumentieren.
7. Code für beide Kerne generieren und beide Ziele zunächst ohne Anwendungserweiterung bauen/laden/starten. M7 enthält nach Boot nur den vorbereiteten, unbenutzte Ressourcen nicht verändernden Hintergrund. Prüfen Sie, dass CM4 seine GPIO-/Timerinitialisierung erreicht und die HAL-Zeitbasis fortschreitet. Halten Sie beide Images nur zur Fehlersuche an; Debughalt ist keine normale Timing-Bedingung.

## Anwendung integrieren

`lab01_demo.c` ausschließlich in das tatsächlich verwendete **CM4-CMake-Ziel** aufnehmen und `code/lab01` als Include-Pfad ergänzen. Der Header verlangt `CORE_CM4` und lehnt `CORE_CM7` ab. CM7 bekommt keinen dieser Anwendungsaufrufe. Die Einbindung eigener Dateien darf bei erneuter CubeMX-Generierung nicht verloren gehen; nötigenfalls eine vom Generator erhaltene eigene CMake-Datei einbinden. Die realen Dateinamen der erzeugten Ziele sind maßgeblich.

In CM4 `main.c` im vorhandenen Anwender-Include-Bereich:

```c
#include "lab01_demo.h"
```

Nach **generierter** GPIO- und TIM2-Initialisierung, im vorhandenen `USER CODE BEGIN 2`-Bereich:

```c
if (!Lab01_Init(LAB01_BASELINE, &htim2)) {
    Error_Handler();
}
```

In den Rumpf der **bereits vorhandenen** `while (1)`, im dazugehörigen Anwenderbereich:

```c
Lab01_Step();
```

Im vorhandenen `HAL_TIM_PeriodElapsedCallback` **einmal** delegieren:

```c
Lab01_TimerCallback(htim);
```

Falls noch kein eigener Callback existiert, genau eine Definition im vorgesehenen Anwenderbereich ergänzen:

```c
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    Lab01_TimerCallback(htim);
}
```

Bei vorhandener HAL-Timer-Zeitbasis deren `HAL_IncTick()`-Behandlung und weitere erforderliche Timerbehandlung erhalten. Nicht zusätzlich einen zweiten gleichnamigen Callback definieren. Der Anwendungsbaustein ignoriert Nullzeiger und andere Timerinstanzen.

## Vier getrennte Läufe

Ändern Sie **nur das erste Argument von `Lab01_Init`**, bauen Sie neu und starten Sie nach Reset. Die Modusauswahl bleibt während eines Laufs unverändert. Vor dem Timerstart setzt der Baustein den Zähler auf null und löscht ein durch Initialisierung bereits anstehendes Updateflag; dieses alte Ereignis gehört nicht zur Messreihe. Kein weiterer LED-Code und kein zusätzlicher `HAL_Delay` darf parallel zum Baustein aktiv bleiben.

| Auswahl | Ausführung |
|---|---|
| `LAB01_BASELINE` | Main toggelt, dann `HAL_Delay(500)`, keine Zusatzlast |
| `LAB01_POLL_CONSTANT` | Main toggelt, 800000 Zähliterationen, dann Delay |
| `LAB01_POLL_VARIABLE` | Main toggelt, abwechselnd 400000/1600000 Iterationen, dann Delay |
| `LAB01_TIMER` | Main nur variable Last; TIM2-Callback toggelt |

Die Ausgangsschleife illustriert die periodische Main-Arbeit einer Super-Loop; ein realer Sensoreingang wird nicht zusätzlich implementiert. Die lokale Lastvariable ist `volatile`: ihre Lese-/Schreibzugriffe dürfen nicht vollständig entfernt werden. **Iterationen sind keine Millisekunden.** Im Delay-Modus enthält der Flankenabstand zusätzliche Last, HAL-Tickquantisierung und Aufrufaufwand. Beim Timer-Modus enthält die Softwareflanke die jeweilige Timer-Handlerverzögerung. Hardwareupdates und beobachtete Callback-Aufrufe müssen bei langsamer/gesperrter Bedienung nicht eins zu eins übereinstimmen. Keiner der Modi behauptet eine harte Fristgarantie oder Jitter null.

`lab01_diag` enthält Main-Schritte, letzte Iterationszahl und TIM2-Callback-Aufrufe. Jeder Zähler hat einen Schreiber. Die 32-Bit-Zähler laufen bei Überlauf modulo 2³² weiter; sie sind für kurze Versuche gedacht. Einen konsistenten Gesamtstand lesen Sie nur bei angehaltenem CM4. `volatile` ist hier für Diagnosezugriffe und den unverändert übernommenen Steuerzustand gedacht, kein allgemeines Verfahren zur sicheren Datenübergabe zwischen Kernen. Es gibt keinen gemeinsamen Datenblock und keine M7-Anwendungskommunikation.

## Beobachten und berichten

Für einen Zahlenvergleich die aufeinanderfolgenden Umschaltungen an PE1 mit geeignetem Oszilloskop/Logikanalysator und geprüftem Messanschluss erfassen. Boardmasse als gemeinsamen Bezug verwenden. Beide Flanken zählen für den Toggle-Abstand, nur gleichgerichtete Flanken für den vollständigen Blinkzyklus. Trigger, Zeitauflösung, Zahl der ausgewerteten Abstände, Modus, Build und Takte dokumentieren. Minimum, Maximum, Mittelwert und Peak-to-Peak-Spanne **erst nach realer Messung** eintragen.

Ohne Messgerät bleibt eine qualitative Beobachtung zulässig: Die LED blinkt oder bleibt aus. Eine Aussage über Mikrosekundenjitter oder sichere Ausführungsgrenzen folgt daraus nicht. Fehlt ein kompletter Aufbau, lesen Sie den Code und planen den Versuch; markieren Sie Hardwareprüfung und Zeitmessung als **nicht durchgeführt**. Keine Modellwerte als Messergebnisse einsetzen.

## Quellen und Prüfgrenzen

Boardreferenz: ST **UM2408**, Timer/Boot: **RM0399** und das tatsächlich generierte CubeH7-Template, Kern: **PM0214**. Der Entwicklungsweg verwendet [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) und [STM32 VS Code Extension](https://www.st.com/en/development-tools/stm32-vscode.html). Die aktuellen Dokumentrevisionen sind am eigenen Projekt zu prüfen.

Ausgeführt wurden eine C11-Syntaxprüfung mit `-Wall -Wextra -Werror` sowie 13 Host-Szenarien mit den echten ST-HAL-/CMSIS-Headern: vier Modi, ungültige Auswahl, fehlendes/falsches Timer-Handle, drei Timerstart-Fehlerstatus und beide Zählerüberläufe. Wiederholte Initialisierung und das Ignorieren anderer Timer/Nullzeiger wurden ebenfalls geprüft. Das falsche Kernziel wird erwartungsgemäß abgewiesen. Die HAL-Aufrufe und das Timerregisterfenster sind im Hostlauf modelliert, keine laufende STM32-Peripherie. Ausgenommen wurden nur die durch 64-Bit-Hostzeiger entstehenden Zeigerbreitenwarnungen; der Anwendungscode verwendet 32-Bit-Registerwerte.

Headerstände: ST HAL `7e541d92019e18f98d211fc4ab9197ec8e8105f6`, STM32H7 CMSIS-Device `81db1ec63cdc191fae1565b772da3ea5aa29a683`. Diese Prüfung ersetzt weder einen ARM-Linklauf mit generiertem Startup/Linkerskript noch einen Boardstart oder die Timer-Taktprüfung am Board. **ARM-Build, Boardtest und Zeitmessung wurden in der Cloud nicht durchgeführt.**
