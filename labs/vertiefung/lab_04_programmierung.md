# Freiwillige Programmiervertiefung zu Übungsblatt 4

Diese Anleitung ist für die **Vorbereitung durch die Lehrperson** oder eine **freiwillige Vertiefung außerhalb des Übungstermins** gedacht. Sie gehört nicht zum verbindlichen Papierkern. Die Zeitangaben der folgenden Abschnitte gelten allenfalls für einen bereits vollständig vorbereiteten, funktionierenden Projektstand; sie sind keine Zusage für Erstintegration und Fehlersuche.

Verwenden Sie STM32CubeMX und VS Code mit STM32 VS Code Extension für das NUCLEO-H755ZI-Q / STM32H755ZIT6. Die Quellen in `code/lab04/` sind Anwendungsbausteine; ein vollständig erzeugtes, auf dem Board geprüftes Firmwareprojekt wird hier nicht bereitgestellt. Versionen, Startfolge und tatsächliche Integration müssen vor der Demonstration geprüft werden.

Im Übungstermin wird höchstens eine kurze vorbereitete Demonstration gezeigt. Bei einem technischen Problem wechseln Sie spätestens nach drei Minuten zum vollständig angegebenen Papierfall im [Übungsblatt](../lab_04_race-conditions-mutex-priority-inversion.qmd). Studierende müssen frühere Programmiervertiefungen nicht erfolgreich abgeschlossen haben, um dem nächsten Übungsblatt folgen zu können.

Wer freiwillig selbst integriert, dokumentiert nur den tatsächlich erreichten Stand. Modellwerte werden nicht als Boardmesswerte eingetragen. Die folgenden technischen Erläuterungen sind offen lesbar und unterstützen die Vorbereitung; die klausurnahen Musterlösungen bleiben im Profil `solution` des Übungsblatts.

## Teil B: Kontrollierter Programmversuch

### Aufgabe 5: CM4-Projekt und Anwendungsbaustein integrieren

Verwenden Sie den **funktionsfähigen Dual-Core-Grundstand aus Vorlesung 3**. In STM32CubeMX behalten Sie Board und Takte sowie FreeRTOS nur auf CM4 über CMSIS_V2 bei. Bauen und debuggen Sie die CMake-Ziele mit **VS Code und STM32 VS Code Extension**. Ein aktueller Generator muss die verwendete Kombination unterstützen; die einmalige Einrichtung wird nicht durch eine behauptete versionsunabhängige Menüfolge ersetzt.

1. Sichern Sie das bisherige Projekt. Halten Sie CubeMX-, CubeH7-, Compiler-, Extension-, Kernel- und Adapterversion fest. M7 bleibt beim dokumentierten Grundstart. Beide passenden Images werden geladen.
2. Verwenden Sie **nur drei Anwenderthreads** für diesen Versuch:

   | Rolle | CMSIS-Priorität | Stackattribut |
   |---|---|---:|
   | L | `osPriorityNormal` | 1024 Byte |
   | M | `osPriorityAboveNormal` | 1024 Byte |
   | H | `osPriorityHigh` | 1024 Byte |

   Die Stackwerte sind Ausgangsgrößen, keine bewiesenen Minima. Entfernen oder deaktivieren Sie andere Anwenderlast, die alte HeavyMath-Schleife und kernelbezogene Tastercallbacks aus Vorlesung 3. Der Taster-IRQ bleibt für diesen Versuch gesperrt. Eine zusätzliche hochbereite Aufgabe würde den Vergleich ändern.

3. Prüfen Sie Präemption, 32-Bit-Kernelticks, 1000-Hz-Labortick, dynamische Objektanlage sowie Mutex-/Semaphorunterstützung in der erzeugten Konfiguration. Prüfen Sie die Prioritätsabbildung in `cmsis_os2.c`; die drei Rollen müssen tatsächlich verschieden und wie angegeben geordnet sein. Der Heap muss Threads, Gates, Testobjekt und übrigen Kernelbedarf tragen. Kontrollieren Sie erfolgreiche Anlage, Map-Datei und gegebenenfalls Stackreserve, statt eine beliebige Heapgröße als Nachweis einzusetzen.

4. Kopieren Sie `code/lab04/lab04_resources.c` und `lab04_resources.h` in einen eigenen CM4-Anwendungsordner. Fügen Sie Quelle und Include-Verzeichnis dem CM4-CMake-Ziel hinzu, über dessen vorhandene Benutzer-Erweiterungsstelle. Die Quelle benötigt die erzeugten Header `main.h` und `cmsis_os2.h`; sie ist kein selbstständiges Firmwareprojekt.

5. Im erhaltenen Benutzer-Include-Bereich der Threaddatei binden Sie `lab04_resources.h` ein. Ersetzen Sie die Körper der vorhandenen generierten Eintrittsfunktionen jeweils durch genau einen passenden Aufruf:

   ```c
   /* In der jeweils passenden generierten Funktion, nicht dreimal in einer */
   Lab04_Low(argument);    /* L */
   Lab04_Medium(argument); /* M */
   Lab04_High(argument);   /* H */
   ```

   Verwenden Sie den tatsächlich generierten Argumentnamen. Jede Funktion kehrt nicht zurück. Es wird **keine zweite Definition** derselben Eintrittsfunktion erzeugt. Code steht in Funktions-USER-CODE-Bereichen, nicht in `Header_...`-Kommentaren.

6. Rufen Sie **nach dem vorhandenen `osKernelInitialize`, vor Anlage und Start der drei Threads** in einem erhaltenen Initialisierungsbereich einmalig auf:

   ```c
   Lab04_Init(LAB04_SEMAPHORE);
   ```

   Der Kernel ist dabei `osKernelReady`. Der Baustein legt die Gates und den Testschutz an und prüft jedes Handle. Prüfen Sie auch alle generierten Threadhandles auf `NULL`. Der vorhandene `osKernelStart` bleibt einmalig; Rückkehr gehört in die Fehlerbehandlung. Eine Fehlerbehandlung darf nicht mit ungültigen Objekten weiterlaufen.

7. Lesen Sie die Startlogik und erklären Sie sie vor dem Flashen: H und M warten an **eigenen** Gates. L erwirbt den Testschutz und öffnet Hs Gate. H öffnet Ms Gate, bleibt zunächst höher als M und fordert dann den belegten Testschutz. Erst jetzt entscheidet die Vererbung, ob M oder L weiterarbeitet. Das zusätzliche dauerhaft leere Parking-Gate parkt beendete Einmal-Threads; `osDelay(osWaitForever)` wird nicht als Ersatz für diesen Objektwartevertrag benutzt.

8. Regenerieren und bauen Sie beide benötigten Ziele. Prüfen Sie, dass Ihre Benutzerbereiche und die CM4-CMake-Einbindung erhalten bleiben. Laden Sie die passenden Images und führen Sie den Semaphorversuch einmal aus.

**Abgabe:** Versionen, `.ioc`, Quellen, Build-/Debugkonfiguration und Ergebnis der Regenerierungsprüfung. Bei fehlender Hardware oder nicht unterstützter Werkzeugkombination dokumentieren Sie den konkreten Stand; Teil A bleibt vollständig lösbar.


Die Eintrittsfunktionen werden den drei Rollen eindeutig zugeordnet; die gezeigten drei Aufrufe sind Alternativen für drei verschiedene Funktionen. `Lab04_Init` wird einmal pro Neustart im initialisierten, noch nicht gestarteten Kernel aufgerufen.

Die beiden Start-Gates und das Parking-Gate beginnen leer. Das Ressourcentoken beginnt bei eins. L hält es bereits, wenn H freigegeben wird. H macht M vor seinem eigenen Blockieren bereit. Dadurch entsteht ohne Vererbung die Auswahl M vor L; beim vererbenden Mutex wird L durch Hs Warten angehoben. Das ist derselbe Mechanismus wie auf Papier, aber **nicht** automatisch dieselbe Freigabe bei 1/2 ms oder dieselbe Millisekundendauer.

### Aufgabe 6: Derselbe Versuch mit Mutex

1. Ändern Sie ausschließlich den Modusaufruf zu `Lab04_Init(LAB04_MUTEX);`. Alle Iterationszahlen, Threadprioritäten, Gates, Takte und Optimierungen bleiben identisch.
2. Bauen, laden und **starten Sie kontrolliert neu**. Der Baustein ist ein Einmalversuch, kein Ablauf zum mehrfachen Initialisieren während eines laufenden Kernels.
3. Lesen Sie im Baustein die beiden vollständigen Objektpfade: Semaphor-New/Acquire/Release beziehungsweise Mutex-New/Acquire/Release. Ein Handle-Cast oder Umbenennen allein genügt nicht. Das Mutexattribut fordert `osMutexPrioInherit` an. Dessen Weglassen ist im geprüften öffentlichen FreeRTOS-Adapter kein verlässlicher Weg zum Abschalten der Vererbung; prüfen Sie Ihren lokalen Adapter.
4. Vergleichen Sie die Debugvariablen erst nach dem Lauf:

   | Beobachtung | Bedeutung |
   |---|---|
   | `lab04_log[LAB04_LOW]` | Ls eigene Zeit- und Zustandsmarken |
   | `lab04_log[LAB04_MEDIUM]` | Ms erster Lauf und Arbeitsende |
   | `lab04_log[LAB04_HIGH]` | Hs Anfrage, erfolgreicher Erwerb, Ende |
   | `lab04_low_effective_priority` | Ls beobachtete wirksame Priorität nach H-Freigabe |
   | `lab04_result[...]` | Ergebnis des identischen begrenzten Rechenpensums |

   Jeder Thread schreibt nur seinen eigenen Log- und Ergebnisplatz. Hält ein Debugger den Prozessor an, verändert er das Zeitverhalten; solche Schritte sind keine ungestörte Zeitmessung. Ein Zustand `LAB04_LOCK_REQUEST` allein beweist keinen Kernelzustand Blocked: Kontrollieren Sie bei Bedarf die tatsächlichen Threadzustände beziehungsweise Traceereignisse.

5. Bestimmen Sie aus Hs Markern das beobachtete Lockintervall `acquired_tick - request_tick`. Vorzeichenlose Differenzbildung berücksichtigt einen einzelnen Zählerumlauf, sofern das Messintervall kleiner als eine volle Zählerperiode ist. Bei 1000 Hz ist ein Tick eine nominale Millisekunde. Gleiche Tickwerte lassen kurze Ereignisse nicht auflösen und beweisen keine Nullwartezeit.

6. Für ein **Marker-Antwortintervall** verwenden Sie Hs `done_tick` minus Ls `gate_release_request_tick`. Dieser Freigabemarker liegt **vor** dem Gate-Release-Aufruf. Das Intervall enthält daher dessen Kosten und ist nicht identisch mit einer isolierten Thread-CPU-Zeit. Legen Sie den Bezug explizit fest; setzen Sie nicht Hs Startmarker mit seiner Freigabe gleich.

7. Ls `release_request_tick` liegt vor dem Test-Release-Aufruf. Ls `done_tick` wird erst nach dessen Rückkehr geschrieben; da H oder M vorher laufen können, ist dieser Marker **nicht** die exakte Ressourcenfreigabezeit. Erfinden Sie daraus keine ungeprüfte Haltedauer.

8. Tragen Sie die tatsächlichen Werte ein. Lassen Sie ungemessene Felder offen. Falls Auflösung oder Ablauf zweifelhaft sind, verwenden Sie geeignete Traceereignisse oder getrennte geprüfte GPIO-Marker. GPIO ist ein allgemeiner Ein-/Ausgangspin; wählen Sie Belegung und Messmethode am Board. Live-`printf` bleibt aus diesem Timingvergleich entfernt.

| Kontrollfeld | Semaphor | Mutex |
|---|---|---|
| L hält Testschutz vor H? | | |
| H wartet tatsächlich am Testschutz? | | |
| M-Gate vor Hs Warten geöffnet? | | |
| Arbeitspensen und Build identisch? | | |
| Ls beobachtete wirksame Priorität | | |
| Hs Lockintervall, Einheit / Auflösung | | |
| Hs Marker-Antwortintervall, Bezug | | |
| Beobachtung / Messgrenze | | |

**Diskussion:** Welche Beobachtung spricht für entfernte M-Verdrängung im Haltefenster? Warum ersetzt ein bestandenes Ergebnis weder den kompletten Firmware-/Portnachweis noch eine allgemeine Deadlineanalyse?


Ohne Vererbung ist L nach seiner Gatefreigabe weiter normal priorisiert, M liegt darüber und kann vor Ls weiterem Ressourcenabschnitt rechnen. Mit dem vererbenden einzelnen Mutex sollte L im jeweiligen geprüften Adapter die hohe effektive Priorität von H erhalten und den Abschnitt vor M beenden können. H wartet weiterhin auf Ls verbleibende Arbeit.

Die Quelle enthält begrenzte 200000, 800000 und 1000 Iterationen für L, M und H. Daraus sind ohne Messung keine 5, 20 oder 1 ms ableitbar. Jeder Ergebnisplatz wird nur vom zugehörigen Thread bearbeitet; `volatile` erhält die Speicherzugriffe und ist kein allgemeines Lock. Die Gatephasen und Protokollierung erzeugen andere nichtnull Kosten als das idealisierte Papiermodell.

Erwartet wird ein dokumentierter Verlauf mit tatsächlicher Ressourcenabhängigkeit und unverändertem Arbeitspensum. Verglichen werden nicht nur zwei Endzahlen. Die Werte 24/4 ms und 25/5 ms gehören zur Papieraufgabe. Auch eine eindeutig verbesserte beobachtete H-Reaktion beweist nur den untersuchten Lauf und die dabei geprüften Bedingungen.
