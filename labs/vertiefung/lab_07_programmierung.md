# Freiwillige Programmiervertiefung zu Übungsblatt 7

Diese Anleitung ist für die **Vorbereitung durch die Lehrperson** oder eine **freiwillige Vertiefung außerhalb des Übungstermins** gedacht. Sie gehört nicht zum verbindlichen Papierkern. Die Zeitangaben der folgenden Abschnitte gelten allenfalls für einen bereits vollständig vorbereiteten, funktionierenden Projektstand; sie sind keine Zusage für Erstintegration und Fehlersuche.

Verwenden Sie STM32CubeMX und VS Code mit STM32 VS Code Extension für das NUCLEO-H755ZI-Q / STM32H755ZIT6. Die Quellen in `code/lab07/` sind Anwendungsbausteine; ein vollständig erzeugtes, auf dem Board geprüftes Firmwareprojekt wird hier nicht bereitgestellt. Versionen, Startfolge und tatsächliche Integration müssen vor der Demonstration geprüft werden.

Im Übungstermin wird höchstens eine kurze vorbereitete Demonstration gezeigt. Bei einem technischen Problem wechseln Sie spätestens nach drei Minuten zum vollständig angegebenen Papierfall im [Übungsblatt](../lab_07_tracing-timing-analyse.qmd). Studierende müssen frühere Programmiervertiefungen nicht erfolgreich abgeschlossen haben, um dem nächsten Übungsblatt folgen zu können.

Wer freiwillig selbst integriert, dokumentiert nur den tatsächlich erreichten Stand. Modellwerte werden nicht als Boardmesswerte eingetragen. Die folgenden technischen Erläuterungen sind offen lesbar und unterstützen die Vorbereitung; die klausurnahen Musterlösungen bleiben im Profil `solution` des Übungsblatts.

## Teil B: lokaler Zykluszählerversuch

### Aufgabe 5: die vorbereitete Projektbasis prüfen

1. Öffnen Sie die tatsächliche .ioc-Datei in STM32CubeMX und die beiden CMake-Ziele in VS Code mit STM32 VS Code Extension. Board und Bauteil entsprechen NUCLEO-H755ZI-Q und STM32H755ZIT6. Notieren Sie CubeH7-, HAL-, CMSIS-, FreeRTOS-, Compiler- und Erweiterungsversionen sowie Optimierung und beide ELF-Dateien.
2. Übernehmen Sie den bereits geprüften Boot-/MPU-/Shared-Memory-Vertrag aus Vorlesung 6. Cortex-M7 bleibt privilegiertes Bare Metal, Cortex-M4 verwendet seinen bestehenden Kernel. Ein vorheriger Ping-Pong-Basislauf wird abgeschlossen; die neue Referenz misst nicht nachträglich dessen Roundtrip und nullt seinen Block nicht. Zusätzliche Schreiber und unkontrollierte Ausgabe bleiben außerhalb des Versuchs.
3. Bestätigen Sie den tatsächlichen M7-Takt über Clock-Tree und Register. Aktualisieren Sie `SystemCoreClock` mit `SystemCoreClockUpdate()`. Ein Spezifikationsmaximum oder eine alleinige Hostanzeige ist kein tatsächlicher Labortakt. Während der Aufnahme sind Taktwechsel, Debughalt und Low-Power-/Sleepwechsel ausgeschlossen; normale Interruptarbeit wird dokumentiert.
4. Prüfen Sie im eingebundenen Cortex-M7-CMSIS-Header den DWT-Registertyp einschließlich CYCCNT, CTRL, LAR und LSR. Der Inithelper benutzt TRCENA, CYCCNTENA und die M7-Softwarelock-Konvention. Reservieren Sie die Zählerzuständigkeit mit dem Debugger: keine fremde Rücksetzung während der Aufnahme. Der Helper setzt CYCCNT nicht null und verändert keine Tracekomparatoren.
5. Die Zeitauswertung setzt ein **Intervall kürzer als einen vollen Zählerumlauf** voraus. Die Modulo-Differenz kann zusätzliche volle Umläufe nicht erkennen. Bestätigen Sie vor der Zeitinterpretation den kurzen Arbeitsfall, fortlaufenden Zähler und stabilen Takt, gegebenenfalls mit separater geeigneter Beobachtung. Ist das nicht belegt, geben Sie nur `cycles_mod32` und die offene Annahme an, keine vermeintlich sichere Mikrosekundenzeit.

### Aufgabe 6: das vollständige CM7-Modul integrieren

| Datei | Verwendung |
|---|---|
| `code/lab07/lab07_timing.h` | gemeinsame Deklaration innerhalb CM7 |
| `code/lab07/lab07_timing.c` | genau einmal im CM7-CMake-Ziel |

Die Quellen verwenden die Kernkennung `CORE_CM7`. Sie gehören nicht ins CM4-Ziel. Alle privaten Diagnosevariablen liegen außerhalb der reservierten SRAM4-Shared-Region; keine zweite Definition im Header anlegen. Der vorbereitete Firmwarestart wird nicht durch dieses Modul ersetzt.

Fügen Sie C-Datei und Include-Verzeichnis dem tatsächlichen CM7-Build hinzu. Im M7-Hauptkontext, nach dem geprüften Takt-, Boot- und MPU-/Cache-Start und gegebenenfalls nach dem endlichen Basis-Ping-Pong:

```c
SystemCoreClockUpdate();
if (Lab07_Init(SystemCoreClock) != LAB07_OK) {
    Error_Handler();
}
if (Lab07_Run() != LAB07_OK) {
    Error_Handler();
}
```

`Error_Handler` ist der vorhandene Fehlerpfad Ihrer Projektbasis. Rufen Sie die Funktionen einmal auf, nicht bei jedem Hauptschleifenumlauf. Kein zusätzliches `osKernelInitialize` oder `osKernelStart`. Der M4-Code bleibt eigenständig. Bauen und flashen Sie die richtigen beiden Images mit der installierten STM32-Erweiterung. Prüfen Sie nach CubeMX-Regenerierung erneut die Quellenliste, Benutzerbereiche, Linkerwahl und Include-Pfade.

**Was die Referenz wirklich misst:** Nach einer NOP-Laufprobe erfasst sie sieben Leerfenster und sieben Arbeitsfenster. Zwischen den Zählerlesungen liegen auch DMB/DSB-Barrieren. Work verwendet eine synthetische private volatile-32-Bit-Größe für 10000 Rotier-/Additionsschritte; diese Zugriffe gehören bewusst zur gemessenen Arbeit. Das Ergebnis wird als Digest nach dem Fenster verwendet. Erst nach der Endlesung speichert das Modul den privaten Datensatz. Kein printf, keine Kernelaufrufe, kein Heap und keine Shared-Payload im Fenster.

Das ist ein **lokales Intervall**, keine reine Job-CPU-Zeit, kein IPC-Roundtrip und kein Kontrollfluss-Trace. Die Startanfrage des alten `Lab06_CM7_Run` liegt bereits vor dessen Aufruf. Auch ein Zählerfenster nur um diesen Aufruf würde nicht automatisch die vollständige erste Anforderung begrenzen.

### Aufgabe 7: Diagnose auswerten und Bedingungen berichten

Nach beendetem Logging lesen Sie die private Diagnose mit dem richtigen CM7-Image und einem zum M7-Cache passenden Debugzugriff. Eine rohe SRAM- oder Live-Memory-Ansicht ist nicht automatisch die vom laufenden Kern benutzte Sicht. Halt während der Aufnahme würde deren Bedingungen verändern.

| Feld | Logisches Soll / Interpretation |
|---|---|
| `lab07_status` | LAB07_OK im gültigen Lauf |
| `lab07_count`, `lab07_done` | 14, 1 |
| Sequenz / Art | 1–7 EMPTY, 8–14 WORK |
| `lab07_dropped` | null im unveränderten 14-Einträge-Lauf |
| `core_hz` | bestätigte M7-Frequenz |
| `digest` | EMPTY null, WORK gleiches rechnerisches Resultat |
| `cycles_mod32` | tatsächlich beobachteter 32-Bit-Restwert |

1. Ordnen Sie die sieben Werte je Gruppe zu. Berichten Sie Anzahl, Minimum und beobachtetes Maximum, zunächst in Taktresten. Rechnen Sie nur bei gültigen Zähler-/Takt-/Umlaufbedingungen mit $t=\Delta c/f_{Kern}$ um.
2. Berichten Sie den Leerwert separat und erklären Sie, welche Kosten zum Fenster gehören. Subtrahieren Sie nicht unbemerkt einen fremden Minimalwert von jedem Arbeitswert und behaupten dann eine reine WCET.
3. Dokumentieren Sie die beobachtete Streuung, tatsächlichen Takt, Optimierung, Cache/MPU, Last, Interruptbedingungen und Debugzustand. Ein gleiches Digest belegt dasselbe berechnete Ergebnis, keine gleiche Laufzeit und keine vollständige Fristgarantie.
4. Der Logvertrag hat sechzehn Plätze und einen Schreiber. Als optionale **Kapazitätsprüfung nach dem abgeschlossenen Basislauf** dürfen Sie im selben M7-Hauptkontext zwei weitere `Lab07_RecordOne(LAB07_EMPTY)`-Aufrufe ausführen. Ein dritter Zusatzaufruf meldet `LAB07_FULL`, erhöht den Dropzähler und überschreibt die ersten sechzehn Datensätze nicht. Dies sind zusätzliche Versuchsaufrufe, kein Fehler des unveränderten 14-Einträge-Laufs. Keine parallelen ISR- oder Threadschreiber hinzufügen.
5. Wenn Initialisierung oder Build scheitert, dokumentieren Sie den tatsächlichen technischen Status. Ein nicht durchgeführter Boardlauf bleibt als solcher gekennzeichnet. Mikrosekundenfelder ohne gültige Beobachtung bleiben leer.


**Technische Erläuterung Aufgaben 5–7**

Die Quellen enthalten vollständige Funktionen und Fehlerpfade; die Projektbasis liefert Takt, Boot, HAL/CMSIS und tatsächliche Speicherplatzierung. Der M7-Helper aktiviert den vorgesehenen DWT-Zähler, löst bei Bedarf seinen Softwarelock und vergleicht zwei Lesungen über 128 NOPs. Er verändert keine Komparatoren und setzt CYCCNT nicht zurück. Fehlender Fortschritt ist ein technischer Fehler. Der C-Hosttest kann diese Logik mit Registerersatz untersuchen, aber keine tatsächlichen Cortex-Takte bestätigen.

Das gültige Run erfasst sieben EMPTY- und sieben WORK-Fenster. Die private volatile-Arbeitsgröße verhindert das Wegoptimieren der bewusst beobachtbaren Zugriffe; `digest` wird zusätzlich nach der Endlesung verwendet. Bei Startwert sieben und der gegebenen 32-Bit-Rotier-/Additionsfolge ist der WORK-Digest `0xbfeb1f4b` (unabhängig berechnet). Die tatsächlichen Zählerwerte sind nicht vorgegeben. DMB/DSB und Funktionskosten innerhalb der Marken bleiben enthalten. Logging folgt erst nach dem Fenster, kann aber spätere Cachebedingungen beeinflussen.

Zwei Zusatzaufrufe füllen Plätze fünfzehn und sechzehn. Der nächste wird verworfen: count bleibt sechzehn, dropped wird eins, die vorhandenen Einträge bleiben erhalten. Ein einziger Schreiber und keine gleichzeitige Pufferwiederverwendung sind vorausgesetzt. Das Verfahren ist kein allgemeiner nebenläufiger Logger.

Die Zeitumrechnung braucht eine bestätigte Frequenz, fortlaufenden Zähler und ein Intervall unter einer Umlaufperiode. Ein reines Modulofeld beweist das nicht selbst. Die beobachteten Minima und Maxima bleiben Stichprobenwerte, keine WCET. Kernel-/Exceptionzuordnung und vollständige Jobereignisse wären zusätzliche Informationen für eigene CPU-Arbeit oder Antwortzeit.

## Optional: vorhandenen Hardwaretrace am bekannten Kanal prüfen

Nur mit tatsächlich bestätigtem Gerät und Aufnahmepfad. Führen Sie nicht mehrere unbestätigte Debugserver parallel. Das Werkzeug dient hier zur zusätzlichen Aufnahme; die Firmwareentwicklung bleibt CubeMX und VS Code mit STM32-Erweiterung.

1. Notieren Sie Modell, Revision, Probe/Adapter, Pins, winIDEA-Version, unterstützte Quellen und Zeitbasis. Ein SWD-Debugzugang beweist kein SWO, keinen Paralleltrace und keine gemeinsame Uhr. Anschlussablauf und Pegel richten sich nach den tatsächlichen Geräte- und Boardunterlagen.
2. Ordnen Sie je Kern die geflashte ELF zu. Prüfen Sie zunächst einen bekannten Zugriff mit Kern, Adresse, Breite und Richtung. Fehlendes erstes Paket ist nicht null.
3. Beobachten Sie den unveränderten nicht cachebaren Kanal aus Vorlesung 6: Antworten 1, 3, 5, Endpayload fünf und TURN_M7. Payload liegt bei Offset acht, state bei vier. Bestätigen Sie die tatsächlich erfasste Quelle statt einen DWT-Zugriff als vollständige SRAM-Historie auszugeben.
4. Inter-Core-Abstände benötigen gleichzeitig erfasste Quellen und bestätigte Zeitkorrelation. Zwei getrennte Zähler oder Aufnahmen genügen nicht. M4-Taskzuordnung während einer ISR ist keine Task-CPU-Zeit. Zustandsereignisse und Anforderungsmarken sind zusätzliche Information.
5. Ein cachebarer Fehlerbuild ist ein eigener gemeinsamer Neustart mit nachgewiesenen effektiven Attributen und laufendem D-Cache. Alte Werte oder Timeout sind mögliche Beobachtungen, keine vorgeschriebene Kurve. Kein Nullen des möglicherweise noch benutzten Blocks. Nach dem Vergleich stellen Sie die nicht cachebare Basis wieder her.

Ohne solche Hardware wird die Interpretation mit den Papierfällen bearbeitet und die Aufnahme als nicht durchgeführt protokolliert. Eine erfüllte Datenfolge und eine aufgezeichnete Zeitfrist sind verschiedene Nachweise.
