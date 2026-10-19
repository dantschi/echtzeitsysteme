# Aktuelle Implementierungsaufgabe zu Übungsblatt 9

`led_output.c` ersetzt im CM7-Ziel `lab09_output.c`. Die Studierenden implementieren Hysterese und Publikationslückenanzeige mit LD1/LD2/LD3 selbst; der Datenkern darunter bleibt vorbereitet.

[Projektvorbereitung](../../labs/vertiefung/lab_09_programmierung.md) · [Aufgaben und Musterlösung](../../labs/lab_09_capstone-timing-verifikation.qmd)

## Technische Dokumentation der bisherigen Bausteine

# Endlicher Capstone zu Vorlesung 9

Plattform: **NUCLEO-H755ZI-Q**, M7 ohne Kernel, M4 mit FreeRTOS über CMSIS-RTOS2. Entwicklungsweg: **STM32CubeMX und VS Code mit STM32 VS Code Extension**, zwei getrennte CMake-Ziele. Die vollständigen Anwendungsbausteine ergänzen ein bereits funktionierendes generiertes Dual-Core-Projekt. Sie sind kein fertiges Boardprojekt und keine behauptete Hardwaremessung.

## Dateien und Zuständigkeiten

| Datei | M7 | M4 |
|---|---|---|
| `lab09_capstone.h`, `lab09_shared.c` | ja | ja |
| `lab09_cm7.c`, `lab09_output.c` | ja | nein |
| `lab09_cm4.c`, `lab09_timer.h/.c` | nein | ja |
| `shared-section.ld` | Integrationsfragment | Integrationsfragment |

Fügen Sie die C-Dateien und den Include-Pfad in die **tatsächlich verwendeten** CMake-Ziele ein. Definieren Sie pro Image genau `CORE_CM7` oder `CORE_CM4`. Entfernen Sie die Anwendungsobjekte vorheriger Labore aus diesen Images. Der neue Shared-Block ersetzt deren Layout. Prüfen Sie die im Projekt aktivierten HAL-Module HSEM, CORTEX, TIM und gegebenenfalls UART sowie den CMSIS-RTOS2-Adapter. Es gibt genau einen Mailboxnutzer je Kern; kein anderer Task und keine ISR nutzt HSEM 1. Weitere lokale Nutzer würden zusätzliche lokale Serialisierung benötigen.

## Speicher und Start

1. In beiden echten Linkerskripten SRAM4 ab `0x38000000`, 64 KiB, ausschließlich als gemeinsamen Bereich reservieren. Entfernen Sie vorhandene überlappende Speicheraliase. Private `.data`, `.bss`, Heap und Stacks liegen außerhalb. Integrieren Sie `shared-section.ld` in `SECTIONS`; es ist kein vollständiges Firmware-Linkerskript. Die Struktur hat elf 32-Bit-Wörter, 44 Byte Nutzdaten, 32-Byte-Ausrichtung und eine auf 64 Byte ergänzte `NOLOAD`-Section. Prüfen Sie beide ELF-/Map-Dateien und die Startkopier-/Nulltabellen. NOLOAD allein verbietet keine späteren Startup- oder Debuggerzugriffe.
2. Nutzen Sie die bestehende Bootsequenz aus Vorlesung 6: M4 wartet im vorgesehenen Stopzustand auf das Boot-Schloss, häufig HSEM 0. M7 bestätigt den Zustand und richtet die Takte ein. Fügen Sie **vor der bestehenden M4-Freigabe**, bei ausgeschaltetem M7-Datencache, `Lab09_CM7_ConfigureMemory(free_region)` und `Lab09_CM7_InitShared(LAB09_NORMAL)` ein. Wählen Sie eine wirklich freie MPU-Region mit Vorrang vor widersprechenden Überlappungen. Der Helper prüft Regionenzahl und Cachezustand, aber nicht die gesamte Überlappungsstrategie. Danach darf der Datencache aktiviert werden, dann folgt die bestehende Boot-Freigabe. Keine zweite Bootsequenz ergänzen.
3. M7 verwendet Normal Memory, TEX=1, C=0, B=0, S=1, Lesen/Schreiben erlaubt, Ausführung verboten. Die MPU ist kernlokal; prüfen Sie M4-Erreichbarkeit und -Rechte. HSEM-Besitz und DMB ordnen die Datenzugriffe, ersetzen aber keine Cache-Kohärenz. Dieser Vertrag für ausgerichtete Armv7-M-Wörter ist keine portable ISO-C-Synchronisation.
4. Der Block beginnt mit `magic=0x49504339`, `mode`, `valid`. Es folgen `event_seq`, `pub_seq`, `event_tick`, `payload`, `ack_pub`, `overwritten`, `snapshots`, `producer_done`. Offsets: Ereignisnummer 12, Bestätigung 28, Abschluss 40. Der M7 ist der einzige Initialisierer, der M4 nullt den Block nicht erneut. Ein Moduswechsel benötigt Neubau/Programmierung und gemeinsamen Neustart. Kein Live-Reset des Blocks bei laufendem Partner.

## Normaler Quellenlauf

1. M4 besitzt und initialisiert **TIM2** als endliche Ereignisquelle mit nominal 100 Hz. M7 verwendet diese Peripherie nicht. Die generierte Variable heißt `htim2`; `MX_TIM2_Init` richtet sie vor dem Versuchstask ein, startet sie aber noch nicht. Verifizieren Sie Clocktree, TIMPRE/Busteiler und den **wirksamen TIM2-Takt**, nicht einfach den Kerntakt oder PCLK. Es gilt $f_{\mathrm{event}}=f_{\mathrm{TIM2}}/((PSC+1)(ARR+1))$. Reines Lehrbeispiel: wirksame 200 MHz, PSC=19999 und ARR=99 ergeben 100 Hz. Übernehmen Sie diese Registerwerte nur bei genau diesem bestätigten Timertakt.
2. Nehmen Sie `lab09_timer.h` im Modul mit dem **vorhandenen** `HAL_TIM_PeriodElapsedCallback` auf und fügen Sie dort genau einen Aufruf `Lab09_CM4_TimerCallback(htim)` ein. Er reagiert nur auf TIM2. Bewahren Sie die bestehende HAL-Zeitbasisbehandlung anderer Timer. Die generierte TIM2-ISR muss den richtigen HAL-Timerhandler aufrufen. Wählen Sie ihre NVIC-Priorität gemäß der zulässigen FreeRTOS-Systemaufrufgrenze des konkreten Ports. Der Timercallback benutzt `osMessageQueuePut` mit Timeout null; eine beliebig dringliche ISR darf diese Kernel-API nicht automatisch verwenden. Eine falsche Priorität kann einen Assert auslösen und ist nicht zwingend eine zurückgegebene Fehlermeldung.
3. Der einzige CMSIS-RTOS2-Worker ruft `Lab09_CM4_Run(argument)` auf. Der Kernel läuft mit 1000 Hz Tickfrequenz, die das Modul prüft. Dynamische Queueerzeugung muss unterstützt sein; es wird `osMessageQueueNew(8, sizeof(Lab09_Record), NULL)` verwendet. Reservieren Sie passenden Heap für acht 16-Byte-Nachrichten, Queueverwaltung und die übrigen Kernelobjekte. Die generierte Task benötigt einen ausreichend geprüften Stack. NULL-Attribute behaupten keine statische Speicherbereitstellung.
4. M7 ruft nach der vorhandenen M4-Freigabe einmal `Lab09_CM7_Run(1u)` auf. Er verwendet keine RTOS-API. HAL-Millisekundenzeitbasis und Code müssen fortlaufen. Seine Pause ist ein aktives HAL-Warten außerhalb des Schlosses. Der gesamte Lauf hat einen softwaregeprüften 3000-ms-Timeout. Die M4-Quelle hat eine softwaregeprüfte 2000-Kerneltick-Grenze und der M4-Erwerb je Versuch 20 Kernelticks. Diese Grenzen sind keine Hardware-Watchdogs und beweisen das 1-ms-Publikationsziel nicht.
5. Der Timer erzeugt synthetische Codes 10, 20, …, 200, keine ADC-Messungen. Die Ereignisroutine nimmt höchstens 20 Eingaben an und vergibt `event_seq` **vor** dem Put. Die Queue überträgt die ganze Nachricht. Der Worker publiziert mit HSEM-ProcessID `0x34`; bei ungültigem Code über 1023 oder abgelaufenem Erwerbsversuch zählt er `task_drops`. `pub_seq` steigt nur bei Publikation. Der M7 kopiert und bestätigt mit ProcessID `0x37`. Beide IDs sind Softwarekennungen, keine Core-IDs.
6. Der M4 stoppt nach der letzten abgearbeiteten Eingabe die Annahme und Quelle, löscht die leere Queue, setzt `producer_done` unter HSEM und beendet den Task. Ein noch eintreffender Callback sieht `accepting=0` und benutzt das gelöschte Objekt nicht. Der M7 kopiert bei Bedarf den letzten Stand, filtert lokal und beendet sich. Warten Sie auf **beide privaten `lab09_complete=1`** und prüfen Sie beide Fehlercodes. Ein abgebrochener Lauf ist keine erfolgreiche Schlussbilanz.

## Sättigung und langsamere Kopie

- Für den Sättigungsmodus initialisiert M7 `LAB09_SATURATION`. Beide Images neu bauen/programmmieren und gemeinsam starten. M4 erzeugt zehn korrekte Puts aus seinem Task **vor dem ersten Get**, ohne Timerstart. Der anfänglich leere Puffer nimmt acht Nachrichten an und lehnt zwei mit `osErrorResource` ab. Das ist ein Kapazitätstest; er behauptet weder zehn physische Flanken noch eine validierte ISR-Priorität. Der Worker verarbeitet die angenommenen acht und beendet sich wie oben.
- Für den langsameren Leser verwenden Sie im **Normalmodus** `Lab09_CM7_Run(25u)`. Das verändert die Last; weniger verschieden kopierte Stände und mehr erlaubte Ersetzungen sind möglich. Jede neue Kopie wird nur einmal gezählt. Beide Bilanzen müssen weiterhin passen. Ein exakter Snapshot-Sollwert folgt ohne vollständiges Zeitmodell nicht aus der Pause.
- Der Filter hält höchstens vier tatsächlich kopierte Codes. Er dividiert die Summe durch die vorhandene Fenstergröße, am Start ohne Nullauffüllung, mit Abrundung nichtnegativer Ganzzahldivision. Bei Werten bis 1023 ist die Summe höchstens 4092. Ausgelassene Publikationen werden nicht nachträglich als Eingaben erfunden. Ein Algorithmus, der jede Probe benötigt, braucht eine andere Transportstrategie.

## Finale Bilanz und Diagnose

Kontrollierter Prüfpunkt: Quelle gestoppt, beide Kerne fertig und angehalten. `lab09_stats` und `lab09_publications` gehören zum **M4-Image**, `lab09_results` und `lab09_result_count` zum **M7-Image**. Der gemeinsame Block liefert P=`pub_seq`, S=`snapshots`, O=`overwritten`, U=1 nur bei gültigem unbestätigtem Stand. Im erfolgreichen endlichen Lauf ist U=0.

Es gelten $I=A+D_q+E_{\mathrm{put}}$, $A=Q+F+P+D_{\mathrm{task}}$ und $P=S+O+U$. Q und F sind am erfolgreichen Ende null. Der Normallauf hat I=20, der Sättigungslauf I=10. Ohne weitere Fehler hat Sättigung A=8, Dq=2, P=8. Die Zahl S kann kleiner als P sein. Die gemeinsame Bilanz wird zusätzlich im Code innerhalb der Besitztransaktionen geprüft.

Zähler werden während Übergängen in mehreren Schritten verändert. Ein beliebiges Live-Lesen ist kein atomarer Bilanzpunkt. Auch das Anhalten allein stellt keine Sichtbarkeit privater M7-Cachezeilen für jeden Debugzugriffsweg her. Platzieren Sie für die vorbereitete RAM-Auswertung M7-Diagnosefelder in geeignetem nicht cachebarem **privatem** Speicher, etwa der projektspezifisch reservierten DTCM-Region, oder verwenden Sie einen nachweislich passenden Debugzugriff. Prüfen Sie die tatsächliche Map-Datei; keine privaten Felder zusätzlich in `.shared` legen. M4-Diagnose und nicht cachebare Mailbox werden nicht mit M7-Diagnose verwechselt.

## Optionale serielle Ausgabe

Der Basislauf speichert höchstens 20 Ergebnisse im privaten RAM und erfordert keine serielle Ausgabe. `lab09_output.c` ist dafür vollständig implementiert. Für den Vergleich definieren Sie **nur im M7-Ziel** `LAB09_UART_HANDLE` auf das tatsächlich generierte und initialisierte UART-Handle, beispielsweise `huart3` bei einem dafür vorbereiteten USART3-Projekt. Aktivieren Sie das passende HAL-UART-Modul und konfigurieren Sie für den vorbereiteten Vergleich 115200 Bit/s, acht Datenbits, ein Stopbit und keine Parität (8N1). Pins und virtuelle COM-Verbindung werden anhand der konkreten Boardrevision und Bestückung geprüft; es wird keine Nucleo-Pinbelegung aus einem anderen Board übernommen.

Die Ausgabe formatiert `e`, `p`, `x`, `y` mit Ereignisnummer, Publikationsnummer, Rohcode und Filterwert. Der Puffer ist auf 80 Byte begrenzt, das HAL-Transmit-Timeout auf 20 Millisekunden. Fehler führen zum privaten Fehlercode und `Error_Handler`. Ausgabe und Filter liegen **nach** HSEM-Release. Die blockierende Ausgabe kann die Snapshotrate senken. Sie besitzt keinen automatisch aus dem Snapshotziel abgeleiteten Ausgabezeitnachweis.

## Zeit- und Watchdogprüfung

Die Quellenzeit `event_tick` stammt vom Beginn der M4-Ereignisroutine im Callback und ist ein **M4-lokaler HAL-Millisekundenwert**. Sie ist keine M7-Uhr, kein physischer Flankenzeitpunkt und keine Mikrosekundenmessung. Der Code enthält keine universelle gemeinsame Zeitquelle. Für C3/C4/C6 ist ein passender Messvertrag zusätzlich erforderlich: gemeinsame externe Aufnahme, geeigneter gemeinsamer Hardwaretimer oder belegte Tracekorrelation. Eine lokale DWT-Messung kann eine lokale Haltezeit erfassen, unter den Bedingungen aus Vorlesung 7; zwei Kernzähler werden nicht ohne Korrelation subtrahiert. Dokumentieren Sie die Marke relativ zu Release, Markeraufwand, Kennung und Abdeckung. Fehlende Kopien sind gezählte Ersetzungen, keine Latenz null.

Der Datenkern startet **keinen Watchdog und keinen Supervisor**. C7 ist hier eine Rechen-/Entwurfsaufgabe. Die WWDG2-Station aus `code/lab08` läuft separat. Eine Integration müsste legitime Stille, eigene Fortschrittsfristen beider Kerne, Hardwarefenster, Startup-Budget und das bewusst endliche Versuchsende regeln. Die bloße Existenz des Workers oder eine hohe Eingaberate sind kein Gesundheitsnachweis.

## Quellen und Validierung

Die lokal verfügbaren öffentlichen STM32H7-HAL-/H755-Geräte-/CMSIS-Header werden zur API-Prüfung verwendet. HAL-Referenzstand aus der vorherigen Prüfung: `7e541d92019e18f98d211fc4ab9197ec8e8105f6`, Geräteheader `81db1ec63cdc191fae1565b772da3ea5aa29a683`. Das legt keine Cube-Projektversion fest. AUTOSAR- und CMSIS-Webdokumente konnten in diesem Cloud-Lauf nicht erneut abgerufen werden; sie werden als weiterführende Literatur verlinkt, nicht als neu gelesener revisionsspezifischer Nachweis bezeichnet.

Hosttests prüfen Anwendungslogik mit geskripteten Quellen, Queuefunktionen und Ausschluss. Sie sind keine Cortex-M-Bus-, Scheduler- oder Timingemulation. Ziel-Build, realer Heap/Stack, Clocktree, IRQ-Prioritäten, Cache-/Debugsicht und Boardaufnahme benötigen das generierte Projekt und die Hardware.

Bei dieser Überarbeitung erfolgreich geprüft: Syntax beider Kernzuordnungen einschließlich optionaler UART-Ausgabe gegen die verfügbaren öffentlichen Header, 15 Host-Pipelineszenarien einschließlich verspäteter Abschlüsse und 20 zusätzliche gleichzeitige Positivläufe, beide Bilanzebenen und Filterwerte, TIM2-Callbackzuordnung und Fehlerretouren, UART-Format und Fehlerpfad sowie beide GNU-ld-Layouts einschließlich Ablehnung zusätzlicher Shared-Objekte. Kein ARM-Zielbuild und keine Boardmessung wurden ausgeführt.
