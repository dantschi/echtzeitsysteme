# Freiwillige Programmiervertiefung zu Übungsblatt 6

Diese Anleitung ist für die **Vorbereitung durch die Lehrperson** oder eine **freiwillige Vertiefung außerhalb des Übungstermins** gedacht. Sie gehört nicht zum verbindlichen Papierkern. Die Zeitangaben der folgenden Abschnitte gelten allenfalls für einen bereits vollständig vorbereiteten, funktionierenden Projektstand; sie sind keine Zusage für Erstintegration und Fehlersuche.

Verwenden Sie STM32CubeMX und VS Code mit STM32 VS Code Extension für das NUCLEO-H755ZI-Q / STM32H755ZIT6. Die Quellen in `code/lab06/` sind Anwendungsbausteine; ein vollständig erzeugtes, auf dem Board geprüftes Firmwareprojekt wird hier nicht bereitgestellt. Versionen, Startfolge und tatsächliche Integration müssen vor der Demonstration geprüft werden.

Im Übungstermin wird höchstens eine kurze vorbereitete Demonstration gezeigt. Bei einem technischen Problem wechseln Sie spätestens nach drei Minuten zum vollständig angegebenen Papierfall im [Übungsblatt](../lab_06_multicore-shared-memory-cache-kohaerenz.qmd). Studierende müssen frühere Programmiervertiefungen nicht erfolgreich abgeschlossen haben, um dem nächsten Übungsblatt folgen zu können.

Wer freiwillig selbst integriert, dokumentiert nur den tatsächlich erreichten Stand. Modellwerte werden nicht als Boardmesswerte eingetragen. Die folgenden technischen Erläuterungen sind offen lesbar und unterstützen die Vorbereitung; die klausurnahen Musterlösungen bleiben im Profil `solution` des Übungsblatts.

## Teil B: kontrollierter Ping-Pong auf dem NUCLEO-H755ZI-Q

### Aufgabe 5: vorbereitete Projektbasis prüfen

Die Lehrperson stellt eine lauffähige Dual-Core-Basis aus Vorlesung 3–5 mit geprüftem Start bereit. Im heutigen Versuch verwendet CM7 Bare Metal und CM4 genau einen FreeRTOS-Kernel. Vorherige Lastthreads, Button-/UART-Versuche und zusätzliche Schreiber werden deaktiviert. M7 und M4 dürfen dieselbe gemeinsame Takt- oder Peripheriekonfiguration nicht unkoordiniert ändern.

1. Prüfen Sie die .ioc-Konfiguration in STM32CubeMX für Board und STM32H755ZIT6. Erfassen Sie tatsächliche Kerntakte, Startadressen, Bootoptionen und Softwareversionen. Spezifikationsmaxima sind keine Laborfrequenzen.
2. Prüfen Sie die beiden CMake-Ziele in VS Code mit STM32 VS Code Extension. Startups, Vektortabellen und Linkerdateien gehören jeweils zum passenden Ziel. Bestätigen Sie die verwendeten Kernkennungen `CORE_CM7` beziehungsweise `CORE_CM4`.
3. Bestätigen Sie den vorhandenen Boot-HSEM-Pfad: M4 bereitet Benachrichtigung vor und geht in Stop; M7 wartet, initialisiert und gibt frei. Stop ist nicht Reset. HSEM-ID, Taktfreigabe, Take-Status und Wartebedingungen werden aus dem konkreten erzeugten Code geprüft. Frühe Pollbudgets sind nicht ohne Zeitbasis als Millisekunden bezeichnet.
4. Halten Sie die SRAM4-Domäne und benötigten Zugriffswege aktiv. Prüfen Sie die laufende HAL-Millisekundenbasis auf CM7 sowie den M4-Kerneltick. Für den nominalen Budgetvergleich wird ein 32-Bit-Kerneltick mit 1000 Hz gewählt. Der M4-Worker erhält zunächst `osPriorityNormal` und 1024 Byte Stack. Heap, Stack und Threadhandle werden am Projekt geprüft.
5. Prüfen Sie den abgestimmten Flash-/Debugstart für beide Images. Ein Halt beider Kerne schon während des Boot-Wartens kann den erforderlichen Stop- oder Wake-Fortschritt verhindern. Starten Sie beide mit dem erprobten Vorgehen der installierten Erweiterung, ohne aus Erinnerung feste GDB-Ports oder einen zweiten konkurrierenden Debugserver einzusetzen.

### Aufgabe 6: gemeinsames Objekt und MPU integrieren

Die Anwendungsdateien enthalten die vollständige Protokoll- und Fehlerlogik. Für die kurze Kursphase sind der freie MPU-Platz und die Linker-/Startup-Anpassung bereits an der Projektbasis vorbereitet. Studierende prüfen die Ergebnisse gegen Teil A und integrieren die zugeordneten Quellen.

| Datei in `code/lab06/` | Ziel und Rolle |
|---|---|
| `lab06_ipc.h` | beide Ziele, gemeinsamer Vertrag |
| `lab06_shared.c` | genau einmal je Image, Definition von `ipc` |
| `lab06_cm7.c` | ausschließlich CM7, MPU/Init und drei Anfragen |
| `lab06_cm4.c` | ausschließlich CM4, ein CMSIS-Worker |
| `shared-section.ld` | Fragment für beide tatsächlichen Linkerdateien |

1. Fügen Sie `.c`-Dateien und Include-Verzeichnis den passenden CMake-Zielen hinzu. Ein kopierter Dateiname allein nimmt die Quelle nicht in den Build auf. Fügen Sie keinen weiteren Definitionsblock für `ipc` hinzu. Die eigenen statischen und Diagnosevariablen gehören in private Regionen außerhalb SRAM4.
2. Reservieren Sie SRAM4 in beiden tatsächlichen Linkerdateien **einmal** für den Kanal. Im gelieferten Fragment heißt die Region `RAM_SHARED`: Ursprung `0x38000000`, Länge 64 KiB. Ersetzen Sie eine bestehende Zuordnung dieses Bereichs, statt einen überlappenden Alias einzufügen. Keine `.bss`, `.data`, Heap oder Stacks in dieser Region. Das Fragment ist keine komplette Linkerdatei und wird in den vorhandenen `SECTIONS`-Block integriert.
3. Prüfen Sie beide erzeugten Maps und ELF-Dateien: `ipc` an `0x38000000`, Typ zwölf Byte, gepolsterte `.shared` 32 Byte und `NOLOAD`. Die Assertions des Fragments müssen bestehen. Prüfen Sie außerdem Copy-/Zero-Tabellen beider Startups auf Ausschluss der Region. `NOLOAD` verbietet keinen beliebigen späteren Schreibzugriff.
4. Wählen Sie eine **tatsächlich freie** MPU-Regionsnummer, deren Priorität oberhalb widersprechender überlappender Regionen liegt. Lassen Sie die vorhandene grundlegende MPU-Politik der Projektbasis erhalten. Der Helfer setzt für den Kanal Lesen/Schreiben und Execute Never und benutzt `MPU_PRIVILEGED_DEFAULT`; das muss zur privilegierten M7-Bare-Metal-Basis passen. Er erkennt nicht automatisch jede Überlappung.
5. Rufen Sie auf CM7 nach dessen vorhandener MPU-Konfiguration und **vor D-Cache-Aktivierung** auf:

```c
Lab06_CM7_ConfigureMemory(shared_region_number, LAB06_NONCACHED);
```

`shared_region_number` ist Ihre zuvor bestätigte freie Nummer. Ersetzen Sie die Angabe durch den tatsächlich gewählten Wert oder eine entsprechende Projektkonstante. Es ist kein universell vorgeschriebener Regionsplatz. Die Referenz prüft numerische Gültigkeit und ausgeschalteten D-Cache. Die effektiven Registerattribute werden zusätzlich kontrolliert.

6. Viele Bootvorlagen haben einen frühen Cachehelper, der I- und D-Cache einschaltet. Für diesen Versuch wird **nur die D-Cache-Aktivierung einmal nach die Shared-Initialisierung gelegt**. Prüfen Sie auch `SystemInit` und weitere MPU-/Cachehelper. Ein nachträglich nochmals aufgerufener MPUhelper darf die Shared-Attribute nicht überschreiben. Die Konfigurationsänderung erfolgt im vorbereiteten Cold-Start-Ablauf, nicht live mit laufender Kommunikation.
7. Fügen Sie nach der gemeinsamen Taktinitialisierung, bei weiterhin ausgeschaltetem D-Cache und vor dem **vorhandenen Boot-HSEM-Release** ein:

```c
Lab06_CM7_InitShared();
SCB_EnableDCache();
/* Danach der vorhandene geprüfte HSEM-Bootpfad. */
```

Die Bootfreigabe wird nicht ein zweites Mal implementiert. Prüfen Sie die konkrete Codeposition: Ein allgemeiner USER-CODE-Block nach der Peripherieanlage kann bereits hinter dem HSEM-Release liegen und damit zu spät sein. Die Initialdaten werden so selbst für den späteren Fehlerbuild zuerst im SRAM bereitgestellt.
8. Rufen Sie `Lab06_CM7_Run()` nach der vorhandenen M4-Freigabe und dem erforderlichen lokalen Start **einmal** auf, nicht bei jedem Umlauf der Hauptschleife. Die Funktion kehrt nach drei Antworten zurück. Der normale Boot- und Fehlerpfad der Basis bleibt erhalten.
9. Auf CM4 ruft die einzige erzeugte IPC-Worker-Einstiegsfunktion `Lab06_CM4_Run(argument)` auf. Übernehmen Sie den tatsächlichen Argumentnamen. Keine zusätzliche gleichnamige Eintrittsfunktion und kein zweites `osKernelInitialize` oder `osKernelStart`. Nach der dritten Antwort beendet die Funktion ihren Thread mit `osThreadExit`. CM4 initialisiert oder nullt `ipc` nicht.
10. Bauen und flashen Sie die beiden passenden Images. Prüfen Sie auch nach erneuter CubeMX-Generierung, ob Quellen, Includes, Linkerwahl und Benutzerbereiche erhalten sind. Ein Buildfehler oder ein nicht durchgeführter Lauf wird entsprechend dokumentiert.


**Technische Erläuterung Aufgaben 5–6**

Die vollständigen Anwendungsfunktionen stehen in `code/lab06/`. Der Datenvertrag bewahrt das aus Vorlesung 6 bekannte Layout: magic/state/payload mit Offsets 0/4/8. Die Kennung ist `IPC_MAGIC`, die Phase ein ausgerichtetes 32-Bit-Wort. Diese Zugriffskonvention ist eine ausdrückliche Armv7-M-/Toolchain-Vereinbarung, keine portable ISO-C-Thread-Synchronisation.

`Lab06_CM7_ConfigureMemory` konfiguriert eine passende Normal-Region mit TEX eins, C null, B null und S eins und erhält die anderen Regionsbeschreibungen. Seine Enable-Politik muss zur Basis passen. Vor jeder Verwendung werden Priorität und effektive Attribute geprüft. `Lab06_CM7_InitShared` verlangt konfigurierte Region und D-Cache aus. M4 ist noch im Boot-Stop; die Funktion setzt Kennung, Payload null und ersten Request geordnet. Erst danach wird D-Cache eingeschaltet und der vorhandene HSEM-Pfad freigegeben.

Auf CM4 startet der Kernel den einzigen Worker. Der Worker prüft zuerst Kennung und Phase, dann nach DMB den erwarteten geraden Anfragewert. Er antwortet mit dem nächsten ungeraden Wert und gibt den Zustand an M7. M7 kopiert die bestätigte Antwort privat und stellt nur die ersten beiden Folgeanfragen bereit. Nach der dritten Antwort bleibt Payload fünf, Zustand TURN_M7. Die private Diagnose wird nicht als zusätzlicher unkoordinierter Shared-Schreiber verwendet.

Zwei getrennte Images dürfen je eine Definition desselben physischen Objekts enthalten. Zwei Definitionen innerhalb eines Images wären dagegen ein Integrationsfehler. Anwendungsquellen allein erzeugen weder Flashstart noch Taktbasis, Linkerskript oder Bootvektoren. Diese projektspezifischen Bestandteile werden am tatsächlich verwendeten Projekt nachgewiesen.

### Aufgabe 7: Basislauf und absichtlichen Fehlerbuild vergleichen

**Basislauf, gemeinsamer Reset:**

- M7-Memorymode `LAB06_NONCACHED`; andere M7-Regionen dürfen D-Cache nutzen.
- Nach Abschluss müssen CM7-Diagnose `lab06_cm7_reply` mit 1, 3, 5 und `lab06_cm7_done` mit eins sowie CM4-Diagnose `lab06_cm4_responses` mit drei und `lab06_cm4_done` mit eins zusammenpassen.
- `lab06_error` ist ein privates Symbol in jedem Image, nicht ein gemeinsamer Fehlerzähler. Beide lokalen Werte sind im gültigen Lauf `LAB06_ERROR_NONE`.
- Gemeinsamer Endblock: gültige Kennung, Payload fünf, Zustand `TURN_M7`. Der M4-Worker ist beendet, der M7 hat keinen vierten Request veröffentlicht.
- Halten Sie für eine konsistente Momentaufnahme alle Kerne an, nachdem beide Abschlusskennzeichen gesetzt sind. Halten verändert das ursprüngliche Timing. Eine angehaltene Gegenstelle mitten im Warten ist kein unabhängiger Cachebeweis.

**Fehlerbuild, eigener gemeinsamer Reset:**

Ändern Sie ausschließlich den M7-Modus bei `Lab06_CM7_ConfigureMemory` auf `LAB06_CACHED_FAULT`. Der Helfer wählt TEX eins, C eins, B eins, S null, also cachebares Normal-Write-Back. Initialisierung bleibt bei D-Cache aus, danach D-Cache tatsächlich einschalten. M4-Code, Layout und Phasenfolge bleiben gleich. Es gibt bewusst keine Cachewartung an diesem gemischten Block.

1. Prüfen Sie effektive MPU-Attribute, aktive Region, Subregionen und Cachebit im M7-CCR, Configuration and Control Register. Fehlende Aktivierung oder eine höhere Überlappung wäre ein anderer Versuch.
2. Dokumentieren Sie die tatsächlich erreichten Antworten, Phase und lokale Fehlercodes. Möglich sind veraltete Werte oder Timeout; ein vollständiger Stillstand oder identischer Fehler in jedem Build wird nicht vorausgesetzt. Bei erfolgreichem Lauf prüfen Sie ebenfalls die Voraussetzungen, bevor Sie einen sicheren Write-Back-Kanal behaupten.
3. Erklären Sie, welche zwei Sichtbarkeitsfehler zum beobachteten Verlauf passen könnten. Eine SRAM-Debugansicht ist nicht automatisch die Kopie, aus der ein laufender M7-Load gelesen hat.
4. Bei Fehler **keine spontane Neuinitialisierung**, keine laufende MPU-Umschaltung und kein pauschales Clean/Invalidate des Blocks. Die andere Seite könnte noch Besitzer sein oder Daten geschrieben haben. Diagnostizieren Sie und starten Sie für eine neue Runde gemeinsam.
5. Stellen Sie `LAB06_NONCACHED` wieder her, bauen Sie neu und prüfen Sie den Basislauf erneut.

| Kontrolle | Eigener Nachweis / Beobachtung |
|---|---|
| tatsächliches Board, Takte, Versionen | |
| Boot-HSEM-ID und Startreihenfolge | |
| beide Maps und Startup-Ausschluss | |
| M7-Region, Priorität und Cachebit | |
| CM4-Tickfrequenz, Threadhandle und Stack | |
| Basis: Antworten und Abschluss beider Kerne | |
| Fehlerbuild: Verlauf und lokale Fehlercodes | |
| wiederhergestellte Basis | |


**Technische Erläuterung Aufgabe 7**

Der gültige Basislauf erfüllt die logische Folge 0 → 1 → 2 → 3 → 4 → 5 mit den drei privaten Antworten 1, 3 und 5. Nach der letzten Antwort bleibt fünf stehen, kein Payload sechs und kein weiterer Request. Genau ein Berechtigter schreibt je Phase. `volatile` und DMB bleiben nötig, obwohl der kleine Kanal nicht cachebar ist.

Im Fehlerbuild kann M7 noch eine alte Zustandskopie lesen, während M4 bereits geantwortet hat. Oder eine Folgeanfrage bleibt nur im Dirty-Cache, während M4 alten SRAM sieht. Cacheverdrängung und konkrete Startreihenfolge können den Fehler verdecken. Ein einmaliger Erfolg widerlegt das Sichtbarkeitsproblem nicht.

Die lokale Timeoutprüfung verlangt fortschreitende Zeit und erneute Codeausführung. `HAL_Delay(1)` ist keine M4-Kernelblockierung; `osDelay(1)` zählt einen Kernel-Tick. Weder deren Parameter noch das Diagnosefenster liefern eine vollständige IPC-Latenz. Der tatsächliche Boardlauf bleibt ohne Projekt/Hardware als nicht durchgeführt gekennzeichnet.

### Zusatzaufgabe: eine echte Zeitmessung definieren

Nur mit passendem Messgerät oder Trace. Legen Sie ein Anforderungsereignis und den geforderten Abschluss fest. Wählen Sie nach Board-/Pinprüfung getrennte GPIO-Marker für den Anfang und das Ende und eine gemeinsame Zeitbasis. Schalten Sie die Marker an den tatsächlichen Veröffentlichungs-/Antwortstellen; berücksichtigen Sie ihre Kosten. Nehmen Sie UART-Ausgabe und absichtliche Anzeige-/Pollingpausen nicht unbemerkt als IPC-Eigenschaft in die Interpretation auf.

Dokumentieren Sie Auflösung, Samplezahl, Last, Kerntakte, Tickrate, Optimierung, Cache-/MPU-Attribute und Debugzustand. Subtrahieren Sie nicht einfach zwei lokale Zykluszähler ohne bestätigte Skalierung und Synchronisation. Der erste Durchlauf kann Boot- und Kernelstart enthalten; spätere Runden sind ein anderer Messfall. Ein endlicher beobachteter Höchstwert ist noch kein allgemeiner Worst-Case-Nachweis.

Ohne Messung bleiben Mikrosekundenwerte leer. Eine sinnvoll definierte Messskizze und die begründete Daten-/Fehlerauswertung sind dann die Abgabe.
