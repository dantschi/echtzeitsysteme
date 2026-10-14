# Freiwillige Programmiervertiefung zu Übungsblatt 5

Diese Anleitung ist für die **Vorbereitung durch die Lehrperson** oder eine **freiwillige Vertiefung außerhalb des Übungstermins** gedacht. Sie gehört nicht zum verbindlichen Papierkern. Die Zeitangaben der folgenden Abschnitte gelten allenfalls für einen bereits vollständig vorbereiteten, funktionierenden Projektstand; sie sind keine Zusage für Erstintegration und Fehlersuche.

Verwenden Sie STM32CubeMX und VS Code mit STM32 VS Code Extension für das NUCLEO-H755ZI-Q / STM32H755ZIT6. Die Quellen in `code/lab05/` sind Anwendungsbausteine; ein vollständig erzeugtes, auf dem Board geprüftes Firmwareprojekt wird hier nicht bereitgestellt. Versionen, Startfolge und tatsächliche Integration müssen vor der Demonstration geprüft werden.

Im Übungstermin wird höchstens eine kurze vorbereitete Demonstration gezeigt. Bei einem technischen Problem wechseln Sie spätestens nach drei Minuten zum vollständig angegebenen Papierfall im [Übungsblatt](../lab_05_deferred-interrupt-processing.qmd). Studierende müssen frühere Programmiervertiefungen nicht erfolgreich abgeschlossen haben, um dem nächsten Übungsblatt folgen zu können.

Wer freiwillig selbst integriert, dokumentiert nur den tatsächlich erreichten Stand. Modellwerte werden nicht als Boardmesswerte eingetragen. Die folgenden technischen Erläuterungen sind offen lesbar und unterstützen die Vorbereitung; die klausurnahen Musterlösungen bleiben im Profil `solution` des Übungsblatts.

## Aufgabe 5: Wertqueue im vorhandenen CM4-Projekt

### Vorbereitung durch die Lehrperson

Verwenden Sie die lauffähige Basis aus Vorlesung 3, mit einem FreeRTOS-Kernel auf CM4 und dem vorhandenen CM7-Systemstart. In STM32CubeMX prüfen Sie CMSIS_V2, Präemption, dynamische Queue-/Semaphorunterstützung und einen 32-Bit-Kernel-Tick mit 1000 Hz. Übernehmen Sie die begründete HAL-Zeitbasis der Basis. Deaktivieren Sie die bisherigen Lastthreads, Semaphore-/Mutexversuche und Buttoncallbacks. Im Wertqueueversuch laufen nur die beiden heutigen Anwendungsthreads zusätzlich zu den Kernelthreads.

Erzeugen Sie Producer mit `osPriorityNormal` und Consumer mit `osPriorityAboveNormal`, zunächst je 1024 Byte Stack. Prüfen Sie Heap und Stack am konkreten Build und unter Laufzeitlast. STM32CubeMX erzeugt die Quellen; Build, Flash und Debug erfolgen in VS Code mit STM32 VS Code Extension für die zugehörigen CM4- und CM7-CMake-Ziele. Es wird kein Wechsel zur eigenständigen STM32CubeIDE verlangt.

### Integration und Durchführung

1. Kopieren Sie `code/lab05/lab05_messages.c` und `.h` in den CM4-Anwendungsbereich. Fügen Sie die `.c`-Datei dem tatsächlichen CM4-CMake-Ziel und ihren Ordner den Include-Verzeichnissen hinzu. Nutzen Sie eine von Cube-Regeneration erhaltene Anpassungsstelle. Ein bloßer Dateikopiervorgang garantiert keine Einbindung in den Build.
2. Fügen Sie `#include "lab05_messages.h"` im vorgesehenen Benutzerbereich ein. Rufen Sie `Lab05_Init()` **einmal nach dem vorhandenen erfolgreichen `osKernelInitialize()` und vor der Threadanlage** auf. Der Kernelstart wird nicht dupliziert. Das Modul prüft alle Handles; ein Fehler stoppt die Anwendung.
3. Die generierte Producer-Einstiegsfunktion ruft nur `Lab05_Producer(argument)` auf, die Consumer-Einstiegsfunktion nur `Lab05_Consumer(argument)`. Legen Sie keine zweiten gleichnamigen Cube-Einstiegsfunktionen an. Der Button-Interrupt bleibt deaktiviert.
4. Bauen und flashen Sie das vorbereitete Dual-Core-Projekt. Starten Sie den Versuch nach Reset. Die Datenqueue besitzt vier Plätze. Der Producer bietet Sequenzen 1–20 mit `sample = 10 * seq` an, verwendet Timeout null und wartet danach jeweils zehn Ticks. Der Consumer entnimmt mit `osWaitForever`, protokolliert und wartet je Datenmessage vierzig Ticks. Beide Wartezeiten sind **keine exklusive CPU-Last** und kein exakt garantiertes periodisches Raster.
5. Nach den zwanzig Versuchen sendet der Producer eine STOP-Nachricht mit wartendem Put. Die STOP-Nachricht hat einen eigenen Typ und zählt nicht zu den Datenversuchen. Der aktive Consumer leert vorherige Nachrichten und parkt nach STOP. Der Producer parkt nach erfolgreicher Übergabe. Niemand released den leeren Park-Semaphor.
6. Halten Sie alle Kerne nach Abschluss an und prüfen Sie `lab05_producer_done`, `lab05_consumer_done`, `lab05_attempt`, `lab05_received`, `lab05_accepted`, `lab05_dropped` und `lab05_received_count`. Interpretieren Sie die Bilanz erst bei beiden Done-Werten eins. Andere als erwartete Kapazitätsfehler führen zum Fehlerpfad und damit zu keinem gültigen abgeschlossenen Versuch.

| Prüfkriterium | Eigene Beobachtung |
|---|---|
| Tickrate und Threadprioritäten | |
| Build/Flash/Start tatsächlich erfolgreich | |
| Beide Done-Werte nach STOP | |
| 20 Datenversuche = angenommen + verworfen | |
| Nach STOP: angenommen = empfangen | |
| Streng steigende empfangene Sequenzen | |
| Inhalt jeder Nachricht: 10 × Sequenz | |
| Welche Sequenzen fehlen laut Producerlog? | |

Ändern Sie für einen zweiten Lauf die Consumerpause im Modul von vierzig auf zehn Ticks, bauen und resetten Sie erneut. Halten Sie Queuegröße und andere Einstellungen gleich. Begründen Sie die beobachtete Veränderung. Gleiche mittlere Wartewerte garantieren wegen tatsächlicher Ausführungszeiten und Tickphase keine beliebige verlustfreie Dauerlast. Halten Sie die beobachteten Zahlen fest, statt die Papierwerte aus Aufgabe 2 einzutragen.


**Technische Erläuterung Aufgabe 5**

Die vollständigen Threads liegen in `code/lab05/lab05_messages.c`. Es werden keine zusätzlichen Schleifen um ihre Aufrufe gebaut; beide kehren im gültigen Ablauf nicht zurück. Im Wertqueueversuch schreiben Producer und Consumer ausschließlich ihre eigenen Diagnosebereiche. Der normale Daten-Put akzeptiert oder verwirft die neue Nachricht. Andere Fehler stoppen den Versuch. Get wird nur nach `osOK` ausgewertet. Die private Wertkopie schützt die empfangenen Zahlen vor Wiederverwendung der Producer-Variable.

Die langsamere Consumerpause erzeugt erwartbar Rückstau und Kapazitätsverluste, deren genaue Anzahl von Tickphase, Scheduling und Build abhängt. Nach STOP und angehaltenem Ablauf gilt $20=\mathrm{accepted}+\mathrm{dropped}$ sowie $\mathrm{accepted}=\mathrm{received}$. STOP steht hinter allen angenommenen Daten und wird separat verarbeitet. Fehlende Endsequenzen sieht man im Producerlog, auch wenn das Consumerlog keinen späteren Wert mehr enthält.

Eine kürzere Consumerpause reduziert den Rückstau unter sonst gleichen Bedingungen. Ohne Boardlauf werden keine Verlustzahl und kein erfolgreiches Flashen behauptet. Tickstempel liefern grobe verstrichene Zeit, keinen Nachweis exklusiver CPU-Ausführungszeit oder einer Mikrosekundenfrist.

## Aufgabe 6: Buttonereignis aus EXTI an einen Worker

Der Buttonversuch wird **nach Reset als eigener Versuch** durchgeführt. Deaktivieren Sie Producer und Consumer. Ein ButtonWorker mit `osPriorityAboveNormal` und zunächst 1024 Byte Stack bleibt. `Lab05_Init()` wird weiterhin einmal vor Threadanlage ausgeführt.

1. Prüfen Sie Boardrevision, Lötbrücken, Pin, Aktivpegel und Flanke anhand der bereitgestellten Unterlagen. Bei bestätigtem PC13 wird dessen EXTI-Leitung dem CM4 zugeordnet. Nutzen Sie in anderen bestätigten Konfigurationen die passende Pinmaske und IRQ. Deaktivieren Sie einen unkoordinierten CM7-Handler derselben Leitung.
2. Prüfen Sie `__NVIC_PRIO_BITS`, die Gruppierung und die Definitionen `configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY` sowie `configMAX_SYSCALL_INTERRUPT_PRIORITY`. Setzen Sie die **logische** EXTI-Priorität auf einen im konkreten Projekt zulässigen Wert. Es gilt nicht allgemein „immer fünf“.
3. Halten Sie diese NVIC-IRQ während GPIO-Initialisierung, Objektanlage und Kernelstart deaktiviert. Falls CubeMX einen frühen Enable-Aufruf generiert, passen Sie die Generierungs-/Benutzerstelle so an, dass kein Zeitfenster für einen frühen Callback bleibt. Die Freigabe erfolgt erst am Anfang der laufenden Button-Einstiegsfunktion, vor `Lab05_ButtonWorker(argument)`.
4. Bei bestätigtem PC13 kann der folgende Code im laufenden Thread verwendet werden. Er verwirft absichtlich vor der Freigabe anstehende Altflags. Konfiguration und Freigabe bilden eine dokumentierte Erfassungsgrenze; ein Ereignis exakt an dieser Grenze ist nicht als sicher erfasst behauptet. Prüfen Sie die Makros und den Handler im lokalen HAL-Paket.

```c
/* Nur für bestätigte EXTI13/EXTI15_10_IRQn-Konfiguration auf CM4. */
__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
Lab05_ButtonWorker(argument);
```

5. Der generierte CM4-IRQ-Handler ruft `HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13)` auf. HAL behandelt das Hardwareflag und ruft den Callback. Fügen Sie den Callback genau einmal außerhalb generierter Funktionstexte hinzu, beispielsweise in einer erhaltenen Benutzer-Code-Sektion. Keine zweite Quittung und kein zusätzliches Yield.

```c
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_13) {
        Lab05_ButtonIRQ();
    }
}
```

6. Das Modul übergibt Sequenz und Kernel-Tick in eine separate Queue mit vier Plätzen. Der Worker liest sie und pausiert fünfzig Ticks. Drücken Sie den Taster zunächst langsam, dann mehrfach. Beobachten Sie nach angehaltenen Kernen `lab05_button_offered`, `lab05_button_accepted`, `lab05_button_dropped`, `lab05_button_errors`, `lab05_button_processed` und `lab05_button_last`. Die Zähler werden nicht in einem parallelen Reportingthread gelesen oder zurückgesetzt. Bei laufender Anwendung ist eine zusammengehörige Momentaufnahme nicht garantiert.
7. Unterscheiden Sie Callbackanzahl, empfangene Nachrichten und echte Betätigungen. Das Modul enthält **keine Entprellung**. Mechanisches Prellen muss nicht in jedem Lauf einen Überlauf erzeugen; ein unbeobachteter Drop wird nicht erfunden. Eine Sequenzlücke zeigt nicht die Zahl vor dem Callback zusammengefasster Flanken.
8. Optional: Beobachten Sie ISR-Eintritt/-Austritt und Workerstart über zusätzliche GPIO-Marker mit Logic Analyzer oder geeignetem Trace. Marker müssen am richtigen Punkt im tatsächlichen IRQ-/Workerpfad sitzen. Dokumentieren Sie Zeitbasis und Messkosten. Ohne Messgerät lassen Sie ISR-Dauer, Workerstartlatenz und Mikrosekundenwerte offen. UART-Ausgabe bleibt außerhalb der ISR und außerhalb der bewerteten Zeitabschnitte.

| Aussage | Beobachtung und Begründung |
|---|---|
| Erlaubte Funktion, Timeout, IRQ-Priorität | |
| Offered = Accepted + Dropped + technische Put-Fehler? | |
| Noch wartende Nachrichten bzw. vollständig geleert? | |
| Callbacksequenz gegenüber Betätigungen | |
| Grenzen der Tickstempel und Diagnose | |

Die zweite Bilanz gilt nur bei gültigem Start und ohne Diagnosezählerüberlauf. `lab05_button_errors` enthält auch erkannte Frühaufrufe, die nicht als angeboten gezählt wurden. Ein solcher Frühaufruf ist ein Konfigurationsfehler und wird vor der Bilanz korrigiert. Alle ISR-Zähler haben genau einen Schreiber, die eine geprüfte CM4-Button-IRQ; `volatile` ersetzt keine allgemeine Synchronisation.


**Technische Erläuterung Aufgabe 6**

Im erlaubten HAL-Pfad genügt der Callback mit `Lab05_ButtonIRQ()`. Der Adapter erledigt bei erfolgreichem Put die erforderliche native Wechselanforderung. Der Worker wird Ready und kann erst nach ISR-Ende und zulässigem Scheduling Running werden. Seine Threadpriorität macht eine zu dringliche NVIC-Priorität nicht zulässig.

Die Queue bewahrt nur angenommene Callbacknachrichten und ist endlich. Bei vollem Kanal zählt der neue Callback als Drop, ältere Nachrichten bleiben stehen. `lab05_button_processed` wird beim erfolgreichen Get erhöht; die anschließende Pause ist nicht Teil einer behaupteten Entprellung. Bei gesperrter Quelle und vollständig entnommener Queue entspricht angenommen der Zahl entnommener Nachrichten. Bei angehaltenem Ablauf mit Rückstau muss dieser Rest berücksichtigt werden. Eine kurze Debuggeranzeige einer LED oder ein erfolgreicher Build ersetzt keine Messung der fertigen Reaktion.

Ein gültiger Start hat keine Frühaufrufe und keine technischen Fehler. Das Diagnosefeld für technische Fehler dient dem Erkennen eines ungültigen Laufs. Es wird in der ISR kein potentiell blockierender Error_Handler aufgerufen. Für die reine Boardbeobachtung bleiben konkrete Messwerte bis zum tatsächlichen Lauf leer.
