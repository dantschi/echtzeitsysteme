# Freiwillige Programmiervertiefung zu Übungsblatt 3

Diese Anleitung ist für die **Vorbereitung durch die Lehrperson** oder eine **freiwillige Vertiefung außerhalb des Übungstermins** gedacht. Sie gehört nicht zum verbindlichen Papierkern. Die Zeitangaben der folgenden Abschnitte gelten allenfalls für einen bereits vollständig vorbereiteten, funktionierenden Projektstand; sie sind keine Zusage für Erstintegration und Fehlersuche.

Verwenden Sie STM32CubeMX und VS Code mit STM32 VS Code Extension für das NUCLEO-H755ZI-Q / STM32H755ZIT6. Die Quellen in `code/lab03/` sind Anwendungsbausteine; ein vollständig erzeugtes, auf dem Board geprüftes Firmwareprojekt wird hier nicht bereitgestellt. Versionen, Startfolge und tatsächliche Integration müssen vor der Demonstration geprüft werden.

Im Übungstermin wird höchstens eine kurze vorbereitete Demonstration gezeigt. Bei einem technischen Problem wechseln Sie spätestens nach drei Minuten zum vollständig angegebenen Papierfall im [Übungsblatt](../lab_03_einfuehrung-freertos-praeemption.qmd). Studierende müssen frühere Programmiervertiefungen nicht erfolgreich abgeschlossen haben, um dem nächsten Übungsblatt folgen zu können.

Wer freiwillig selbst integriert, dokumentiert nur den tatsächlich erreichten Stand. Modellwerte werden nicht als Boardmesswerte eingetragen. Die folgenden technischen Erläuterungen sind offen lesbar und unterstützen die Vorbereitung; die klausurnahen Musterlösungen bleiben im Profil `solution` des Übungsblatts.

## Teil B: Programmieren und Beobachten

### Aufgabe 5: Projekt mit CubeMX und VS Code vorbereiten

**Werkzeuge:** STM32CubeMX, STM32CubeH7-Paket, VS Code mit offizieller STM32 VS Code Extension und deren Build-/Debugwerkzeugen, USB-Verbindung zum ST-LINK des Boards. ST-LINK ist der integrierte Programmier- und Debugadapter. Ein erfolgreicher Dokumentbuild ist kein Firmwarebuild.

1. Notieren Sie die Versionen von CubeMX, CubeH7, Extension, Compiler, FreeRTOS-Kernel und CMSIS-Adapter. Wählen Sie das Board **NUCLEO-H755ZI-Q**, MCU **STM32H755ZIT6**. Erzeugen Sie mit dem **CMake-Generator** die Projektteile für Cortex-M7 und Cortex-M4. CMake beschreibt Übersetzen und Verknüpfen. Öffnen Sie das erzeugte Projekt in VS Code und wählen Sie die vorgesehenen STM32-Build-/Debugfunktionen.

   Prüfen Sie vor weiteren Änderungen, dass der installierte Generator die Kombination **Dual-Core, CMake und FreeRTOS auf CM4** unterstützt und tatsächlich zwei passende Ziele erzeugt. Menübezeichnungen und Dateistruktur sind versionsabhängig. Fehlt diese Unterstützung, dokumentieren Sie die konkrete Diagnose und verwenden Sie mit der Lehrperson einen passenden aktuellen Werkzeugstand oder ein bereitgestelltes Dual-Core-CMake-Projekt. Ein nur für CM7 erzeugtes Projekt erfüllt die Aufgabe nicht. Wechseln Sie nicht still zu CubeIDE.

2. Übernehmen bzw. prüfen Sie die Bootfolge aus Vorlesung 1. CM7 enthält die System-/Startfunktion; CM4 die Anwendung. Laden Sie beide passenden Images, prüfen Sie beide Resets und das Erreichen der CM4-Anwendung. M7-400-MHz und M4-200-MHz sind Lehrvorgaben, keine automatisch erzeugten Taktraten. Wenn Sie vorhandene Firmware aus Vorlesung 1/2 verwenden, sichern Sie diese vor Flashänderungen.

3. Konfigurieren Sie FreeRTOS **nur auf CM4**, Schnittstelle **CMSIS_V2**, Präemption aktiv, 32-Bit-Kernelticks, Labor-Tickfrequenz 1000 Hz. Prüfen Sie im erzeugten `FreeRTOSConfig.h` die Werte und im Adapter `cmsis_os2.c` die Prioritätsabbildung. `configMAX_PRIORITIES` muss die gewünschten CMSIS-Prioritäten unterstützen. Wählen Sie die Heapgröße anhand der tatsächlich erzeugten Objekte; 16 KiB kann ein Startwert sein, ist kein bestätigtes Minimum.

4. Laborvariante: SysTick für den Kernel, einen freien **TIM6** für die HAL-Zeitbasis. Prüfen Sie Tickfrequenzen und Handlerketten. Ein gemeinsam korrekt integrierter SysTick wäre möglich, wird in diesem Versuch jedoch nicht verwendet.

5. Konfigurieren Sie **LD2/PE1** und **LD1/PB0** als Ausgänge für CM4. Verifizieren Sie Boardrevision und Belegung gegen UM2408 bzw. den Schaltplan. Konfigurieren Sie Benutzer-Taster **PC13** mit passender Flanke und EXTI-Leitung für CM4. EXTI ist der External Interrupt/Event Controller. Prüfen Sie den elektrischen Ruhepegel, statt eine Flankenrichtung zu raten.

6. Der Taster-IRQ **EXTI15_10** muss für Kernelaufrufe erlaubt sein und zu der in Aufgabe 4 erläuterten Bibliotheksschwelle passen. In diesem Labor darf keine weitere EXTI-Leitung diesen IRQ benutzen. Lassen Sie die NVIC-Freigabe bei der Generierung aus. NVIC bedeutet Nested Vectored Interrupt Controller. Falls CubeMX sie zwingend erzeugt, sperren Sie den IRQ unmittelbar nach der GPIO-Initialisierung in einem erhaltenen USER-CODE-Block, noch bevor ein gültiges Threadhandle vorausgesetzt wird. Er darf vor dem Kernelstart keinen Flagaufruf auslösen.

7. Bauen Sie beide **noch unveränderten** Ziele in VS Code. Kontrollieren Sie Image, Kernzuordnung, Linkerskript und Map-Datei. Dokumentieren Sie die tatsächlichen Erfolgsmeldungen oder Fehler. Eine Map-Datei ist der Speicherbericht des Linkers. Erst nach funktionierendem Dual-Core-Start integrieren Sie die Threads.

**Abgabe:** Versionstabelle, `.ioc`, Build-/Debugkonfiguration sowie Nachweis der beiden erzeugten und geladenen Ziele. Wenn Hardware oder Werkzeugunterstützung fehlt, bleibt Teil A vollständig bearbeitbar; dokumentieren Sie Teil B als nicht durchgeführt.


**Erwartete Kontrolle:** Zwei korrekt zugeordnete Images mit konsistenter Bootfolge, Kernel ausschließlich auf CM4. Der CM4-Kontext erreicht die Kernelinitialisierung. SysTick bedient die konfigurierte Kernelzeit, die erzeugte TIM6-Handlerkette die HAL-Zeitbasis. Ungeprüfte gemeinsame Handler und ein mitaktivierter Kernel auf CM7 sind keine Lösungen. Eine bestimmte hier erfundene Extension-Menüfolge oder ein angeblich getestetes Firmwareprojekt wird nicht vorausgesetzt.

### Aufgabe 6: Drei Threads integrieren

Legen Sie in CubeMX diese drei CMSIS-RTOS2-Threads an. Benennen Sie einen eventuell erzeugten Default-Thread entsprechend um oder entfernen Sie ihn; eine zusätzliche hochpriore Endlosschleife würde den Versuch verändern.

| Rolle | CMSIS-Priorität | Stackattribut in Byte |
|---|---|---:|
| Ereignis-Task | `osPriorityHigh` | 1024 |
| Heartbeat | `osPriorityNormal` | 512 |
| HeavyMath | `osPriorityBelowNormal` | 1024 |

Die Größen sind Ausgangswerte, keine nachgewiesenen sicheren Minima. Prüfen Sie die Einheit der CubeMX-Felder gegen `.stack_size` im generierten Code.

**Anwendungsbaustein:** Kopieren Sie `code/lab03/lab03_rtos.c` und `lab03_rtos.h` aus dem Kursrepository in einen eigenen CM4-Anwendungsordner. Fügen Sie Quelle und Include-Verzeichnis dem **CM4-CMake-Ziel** hinzu, vorzugsweise über dessen vorhandene Benutzer-Erweiterungsstelle. Der Baustein ist nicht für CM7 vorgesehen. Er benötigt die erzeugten Header `main.h` und `cmsis_os2.h`, GPIO-Initialisierung und Kernelkonfiguration. Kontrollieren Sie, dass eine erneute CubeMX-Generierung diese Einbindung und Ihre USER-CODE-Bereiche erhält.

1. Binden Sie `lab03_rtos.h` im erhaltenen Include-Bereich der erzeugten Threaddatei und der Datei mit dem HAL-Callback ein.
2. Ersetzen Sie den generierten Schleifenkörper jeder Eintrittsfunktion durch **einen** Aufruf des passenden Bausteins. Erzeugen Sie keine zweite Definition der Threadfunktion. Beispiel für die vorhandene Heartbeat-Funktion:

```c
/* Innerhalb der generierten Heartbeat-Eintrittsfunktion */
Lab03_Heartbeat(argument); /* tatsächlichen Argumentnamen verwenden */
```

   Analog rufen HeavyMath und Ereignis-Task `Lab03_HeavyMath(argument)` bzw. `Lab03_Event(argument)` auf. Die Funktionen kehren nicht zurück. Code gehört in Funktions-USER-CODE-Bereiche, nicht in `Header_...`-Kommentare.

3. Prüfen Sie nach dem vorhandenen `osKernelInitialize` alle drei von `osThreadNew` erzeugten Handles auf `NULL`. Nach erfolgreicher Anlage, **vor** dem einzigen vorhandenen `osKernelStart`, rufen Sie auf:

```c
Lab03_BindEvent(eventTaskHandle); /* Ihren generierten Handle-Namen einsetzen */
```

   Der Kernel muss zu diesem Zeitpunkt im Zustand `osKernelReady` sein. `osKernelStart` kehrt bei Erfolg in der üblichen Integration nicht zurück; eine Rückkehr gehört in die Fehlerbehandlung.

4. Verwenden Sie den **bestehenden** `HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)`. Fügen Sie darin `Lab03_ButtonIRQ(GPIO_Pin);` ein. Falls noch kein eigener Callback existiert, definieren Sie einmalig die von der HAL vorgesehene Funktion im erhaltenen Benutzerbereich. Definieren Sie keinen zusätzlichen `EXTI15_10_IRQHandler`: Die generierte Interrupt-/HAL-Kette muss weiter genau einmal zuständig bleiben.

5. Lesen Sie den Baustein. Er setzt beim Start der Ereignis-Task alte PC13-Pending-Markierungen zurück und gibt danach den IRQ frei. Diese Startpolitik verwirft Flanken vor Betriebsbeginn. Danach werden angenommene Thread-Flags durch `osThreadFlagsWait` automatisch gelöscht, ohne zusätzliches blindes Löschen.

6. Bauen und laden Sie wieder beide benötigten Images. Die Anwendungsquelle ist ein Lehrbaustein, kein schon auf Ihrem Board getesteter Komplettstand. Prüfen Sie mindestens eine erneute Generierung mit anschließendem Build auf Erhalt der Integration.

**Beantworten Sie zusätzlich:** Warum darf die ISR (Interrupt Service Routine) hier `osThreadFlagsSet`, aber nicht `osThreadFlagsWait` aufrufen? Warum ist `osOK` der falsche Erfolgstest für Flagfunktionen?


**Begründung:** `osThreadFlagsSet` ist für zugelassenen Interruptkontext vorgesehen und wartet nicht. Der FreeRTOS-Port muss die IRQ-Priorität trotzdem erlauben. `osThreadFlagsWait` blockiert den aufrufenden Thread und gehört nicht in eine ISR. Flagfunktionen liefern Flagbits oder einen Wert mit reserviertem Fehlerbit. Daher wird `(flags & osFlagsError) != 0u` geprüft; `osOK` ist ein Statuscode anderer Aufrufe.

**Bausteinverhalten:** `Lab03_Heartbeat` konvertiert 500 ms mit 64-Bit-Zwischenprodukt und Aufrundung in eine positive Tickzahl. `LabWaitNext` erhöht den vorherigen Solltermin. Bei abgelaufenem Ziel zählt es `lab03_target_misses`, richtet auf jetzt plus Periode neu aus und wartet. Ein zwischen Prüfung und Adapteraufruf verpasstes Ziel wird einmal ebenso behandelt; weitere Fehler führen zu `Error_Handler`. Der Zähler bezeichnet Wartezielverfehlungen, keine vollständige Deadlineanalyse.

`Lab03_HeavyMath` bearbeitet eine eigene vorzeichenlose `volatile`-Variable dauerhaft und bleibt Ready. `volatile` erhält Speicherzugriffe, bietet aber keine gemeinsame Datensynchronisation. `Lab03_Event` wartet auf Bit null und schaltet LD1/PB0 um. LD2/PE1 bleibt ausschließlich Heartbeat zugeordnet. Die Ergebnisse `lab03_event_responses` zählen angenommene Hinweise, nicht verlustfrei einzelne physische Tastendrücke.

### Aufgabe 7: Normalbetrieb und Starvation vergleichen

**Normalbetrieb**

- Heartbeat schaltet LD2 ungefähr alle 500 ms um, obwohl HeavyMath nicht wartet. Ein ganzer LED-Zyklus dauert ungefähr eine Sekunde.
- Tasterhinweise schalten LD1 um. Ein mechanischer Taster kann prellen. Das Flag beschreibt **mindestens einen ausstehenden Reaktionswunsch**, keinen garantierten Zähler entprellter Tastendrücke.
- Kontrollieren Sie die Debugvariablen `lab03_accumulator`, `lab03_irq_errors`, `lab03_target_misses` und `lab03_event_responses`. Ihre Werte sind Beobachtungen des tatsächlich durchgeführten Laufs. Halten im Debugger verändert das Zeitverhalten; nutzen Sie die Variablen nicht als ungestörte Zeitmessung.
- Begründen Sie die erwarteten Zustände aller drei Threads zwischen zwei Heartbeat-Aktionen.

**Geänderte Priorität**

1. Ändern Sie nur HeavyMath auf `osPriorityAboveNormal`, also über Heartbeat und unter Ereignis-Task. Vermeiden Sie gleiche Prioritäten. Erzeugen, bauen und starten Sie neu.
2. Dokumentieren Sie LD2 und LD1. Die Heartbeat-LED darf in ihrem letzten Zustand stehen bleiben oder schon vor ihrer ersten Aktion gar nicht beginnen; beides passt zu fehlender Zuteilung.
3. Erklären Sie, warum `osThreadYield()` in HeavyMath Heartbeat nicht rettet. Was passiert dagegen bei echtem Betriebssystem-Warten in HeavyMath?
4. Stellen Sie `osPriorityBelowNormal` wieder her und prüfen Sie den Normalbetrieb erneut.

**Abgabe:** Gegenüberstellung beider Konfigurationen, tatsächliche Beobachtungen und begründete Diagnose. Eine stehengebliebene LED allein ist noch kein Beweis für die Ursache. Prüfen Sie Handle, Kernelstart, Boot, GPIO, Ready-Zustand und tatsächliche Priorität.


Im Normalbetrieb sind Heartbeat und Ereignis-Task zwischen Aktionen Blocked, HeavyMath Running. Bei Freigabe einer höheren Aufgabe ist HeavyMath Ready, während diese läuft. Der Normalversuch demonstriert präemptive Zuteilung; sichtbar ruhiges Blinken beweist keine harte Frist und keinen Jitter von null.

Mit HeavyMath über Heartbeat ist Heartbeat Ready ohne Zuteilung: Starvation. Die Ereignis-Task bleibt höher und kann reagieren. Yield nimmt HeavyMath nicht aus der Ready-Menge. Echtes Warten macht HeavyMath dagegen Blocked und erlaubt Heartbeat die Auswahl, wenn keine noch höhere Aufgabe bereit ist. Dieses Verhalten ist keine Priority Inversion, weil keine Ressource gehalten wird.

### Zusatzaufgabe: Messen statt nur Blinken beobachten

Falls ein Oszilloskop oder Logikanalysator verfügbar ist, legen Sie einen **separaten freien GPIO-Marker** um die Heartbeat-Arbeit. Pin und Boardanschluss müssen anhand des Schaltplans gewählt werden; benutzen Sie dafür nicht die von einem anderen Thread geschriebene LED.

- Unterscheiden Sie Marker-Pulsbreite $w_k=f_k-s_k$ und Startabstand $P_k=s_{k+1}-s_k$. $s_k$ ist die Start-, $f_k$ die Endmarke. Eine Pulsbreite kann Präemption oder Interruptarbeit einschließen.
- Notieren Sie Werkzeugstand, Taktraten, Tickrate, Messpin, Optimierung, Last und Geräteauflösung.
- Erläutern Sie, warum eine gemessene maximale Pulsbreite in dieser endlichen Testreihe noch keine allgemein nachgewiesene größte Ausführungszeit ist.
- Falls keine Messung stattfand, tragen Sie keine Modellzahlen als Messergebnis ein.
