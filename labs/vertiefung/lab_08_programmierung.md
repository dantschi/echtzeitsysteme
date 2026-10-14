# Freiwillige Programmiervertiefung zu Übungsblatt 8

Diese Anleitung ist für die **Vorbereitung durch die Lehrperson** oder eine **freiwillige Vertiefung außerhalb des Übungstermins** gedacht. Sie gehört nicht zum verbindlichen Papierkern. Die Zeitangaben der folgenden Abschnitte gelten allenfalls für einen bereits vollständig vorbereiteten, funktionierenden Projektstand; sie sind keine Zusage für Erstintegration und Fehlersuche.

Verwenden Sie STM32CubeMX und VS Code mit STM32 VS Code Extension für das NUCLEO-H755ZI-Q / STM32H755ZIT6. Die Quellen in `code/lab08/` sind Anwendungsbausteine; ein vollständig erzeugtes, auf dem Board geprüftes Firmwareprojekt wird hier nicht bereitgestellt. Versionen, Startfolge und tatsächliche Integration müssen vor der Demonstration geprüft werden.

Im Übungstermin wird höchstens eine kurze vorbereitete Demonstration gezeigt. Bei einem technischen Problem wechseln Sie spätestens nach drei Minuten zum vollständig angegebenen Papierfall im [Übungsblatt](../lab_08_hsem-wwdg-safe-shutdown.qmd). Studierende müssen frühere Programmiervertiefungen nicht erfolgreich abgeschlossen haben, um dem nächsten Übungsblatt folgen zu können.

Wer freiwillig selbst integriert, dokumentiert nur den tatsächlich erreichten Stand. Modellwerte werden nicht als Boardmesswerte eingetragen. Die folgenden technischen Erläuterungen sind offen lesbar und unterstützen die Vorbereitung; die klausurnahen Musterlösungen bleiben im Profil `solution` des Übungsblatts.

## Labor A · Der endliche Zähler · 25 Minuten

Verwenden Sie **STM32CubeMX und VS Code mit STM32 VS Code Extension für ein NUCLEO-H755ZI-Q**. M7 bleibt ohne Kernel, M4 verwendet FreeRTOS über CMSIS-RTOS2. Ein bereits gemeinsam startendes Projekt und die Speicherbasis aus Vorlesung 6 müssen vorbereitet sein. Die vollständige Integration steht in [code/lab08/README.md](../../code/lab08/README.md).

1. Übernehmen Sie `lab08_counter.h`, `lab08_counter.c`, `lab08_shared.c` in beide tatsächlichen CMake-Ziele. Definieren Sie pro Ziel genau `CORE_CM7` oder `CORE_CM4`. Entfernen Sie vorherige Labor-Anwendungsobjekte. Reservieren Sie HSEM 1 ausschließlich für je einen Worker pro Kern.
2. Der neue Shared-Block **ersetzt** den Vorlesung-6-Block: 28 Byte Nutzstruktur, 32 Byte Section, SRAM4 ab `0x38000000`. Übernehmen Sie `shared-section.ld` in beide tatsächlich verwendeten Linkerskripte, ohne Speicheralias. Prüfen Sie die beiden ELF-/Map-Dateien und dass Startcode/Debugger die Section nicht nachträglich nullen.
3. M7 konfiguriert bei ausgeschaltetem Datencache eine tatsächlich freie MPU-Region als Normal, nicht cachebar und führt `Lab08_CM7_InitShared(LAB08_PROTECTED)` aus, während M4 noch im vorhandenen Boot-Stop wartet. Anschließend darf der Cache an, dann folgt die **bestehende** Bootfreigabe. M7 ruft danach einmal `Lab08_CM7_Run()` auf. Der einzige M4-Worker ruft `Lab08_CM4_Run(argument)` auf.
4. Bauen und programmieren Sie **beide Images**, starten Sie gemeinsam und lassen Sie den endlichen Versuch frei laufen. Erst bei M7-`lab08_complete=1` halten Sie beide Kerne an. Notieren Sie Treffer, Fehlschläge, Fehlercode, Endwert und Erfolgssumme. Bei Timeout kein Ergebnis als abgeschlossenen Lauf bewerten.
5. Für den Vergleich ändern Sie nur die einmalige M7-Initialisierung auf `LAB08_UNPROTECTED`. Beide Images neu bauen/programmieren, gemeinsam neu starten, dieselben Werte notieren. Der Shared-Modus wird von beiden gelesen. Interpretieren Sie auch den möglichen zufällig passenden Endwert. Nie den Zähler bei laufendem Partner neu initialisieren.

| Projektbedingung | Ihre Angabe |
|---|---|
| Boardrevision, CubeMX / HAL / Extension-Version | |
| beide Kern- und Bustakte, M4-Kerneltick | |
| Boot-HSEM / Zähler-HSEM / freie MPU-Region | |
| Shared-Adressen, Größe, Starttabellen geprüft | |

| Lauf | Treffer M7 / M4 | Fehlschläge | Endwert / Sollsumme | Fehler / Erklärung |
|---|---|---|---|---|
| geschützt | | | | |
| ungeschützt | | | | |


**Technische Erläuterung Labor A**

Geschützt gilt bei beiden veröffentlichten Abschlüssen $c=h_7+h_4$. Bei je 100 Erfolgen sind es 200, bei fehlgeschlagenen Takes entsprechend weniger. Die Treffer- und Fehlersummen eines Workers ergeben 100 Versuche. Ungeschützt sind alle 100 Schritte pro Kern als ausgeführt gezählt, aber deren Speicherwirkung kann überlappen. Ein Ergebnis 200 ist möglich und kein Race-Freiheitsbeweis. Es wurden hier keine Boardwerte vorgegeben. Prüfen Sie den privaten Fehlercode pro Image. `HAL_Delay(1)` und `osDelay(1)` gehören zu verschiedenen Zeitbasen; kein garantierter Fairness- oder Timingnachweis folgt aus diesen Pausen.

## Vorbereitete Zusatzstation · WWDG2 und isolierter Ausgang

Diese Station ergänzt den Zählerversuch, wenn ein zweites fertig vorbereitetes Projekt und ein Logikanalysator verfügbar sind. Sie benötigt keine neue Pflicht-Toolchain. Bei engem Zeitbudget zeigt die Lehrperson die drei Fälle. WWDG2 läuft auf M4 in einer **separaten Projektkopie**, ohne Zählerworker und ohne fremdes Füttern. Kein Aktor wird angeschlossen.

- Nutzen Sie `lab08_wwdg.h`, `lab08_wwdg.c`, `lab08_board.c` nur im M4-Ziel. Das Modul startet WWDG2 erst im laufenden Task. Entfernen Sie einen vorherigen generierten `MX_WWDG2_Init()`-Aufruf. APB1-PCLK 100 MHz, Kerneltick 1 kHz, Teiler 8, Fenster 80, Reload 127, EWI an. Andere Watchdogs und deren Startoptionen sind aus.
- Wählen Sie anhand Boardplan drei freie GPIOs mit CubeMX-Labels `LAB_ENABLE`, `LAB_REFRESH`, `LAB_EWI`. Ausgangsstart Low, Takte an, nur isolierte Anzeige/Messpunkte. ENABLE ist aktiv High und benötigt einen externen Pull-down für die Resetphase. Die Anleitung enthält vollständige GPIO-Funktionen und verlangt bewusst keine angenommene Boardpinzahl.
- Die generierte `WWDG_IRQHandler` ruft genau einmal `Lab08_WWDG_IRQHandler()` auf. EWI gehört laut H755-Header zu `WWDG_IRQn`, nicht zu `WWDG_RST_IRQn`. Die Projektpriorität wird in einer statischen `Lab08_WWDG_Config` angegeben. Der Callback benutzt keine Kernel-API, kein Delay und keinen Refresh.
- Testen Sie je Neustart genau einen Modus: `LAB08_WWDG_EARLY` (5 Ticks), `LAB08_WWDG_VALID` (zehn absolute 18-Tick-Ziele, dann bewusst nicht weiterfüttern), `LAB08_WWDG_LATE` (25 Ticks ohne Refresh). Das Modul prüft im gültigen Fall zusätzlich den Zähler vor dem Schreiben. Die kurze Interruptmaskierung hält nicht den Hardwarezähler an und ist kein Timingbeweis.
- Beobachten Sie Marker direkt am Vorgang. EWI kann bei frühem Reset fehlen. Ergänzen Sie `lab08_reset.h/.c` nur im M7-Ziel und rufen Sie `Lab08_CM7_CaptureReset()` früh vor HAL-/Cacheinitialisierung auf. M7 sichert und löscht als einziger die gemeinsame Resetursache; M4 löscht sie nicht. Dokumentieren Sie Debug-Freeze, tatsächlichen freien Lauf und Pinpegel während Reset/Boot. Keine langen Haltepunkte im laufenden Fenster. Diagnosewörter sind nach Reset kein automatisch erhaltenes Log.

| Fall | Modell | Resetursache / Markerzeit / Pegel beobachtet |
|---|---|---|
| früh | Refresh außerhalb Fenster, EWI nicht erforderlich | |
| gültig | zehn akzeptierte Refreshversuche, danach geplanter Reset | |
| spät | EWI falls bedient, anschließend Reset vor 25 Ticks | |


**Technische Erläuterung Zusatzstation**

Die 18-Tick-Ziele sind eine Versuchsplanung, keine Zusage. Mit dem angenommenen 1-kHz-Tick und 100-MHz-PCLK liegt der Zielabstand im Modell im Fenster. Der erste Schritt kann phasenverschoben sein; Aufwach- und Handlerverzug müssen geprüft werden. Der Referenztreiber startet bei Init und prüft bei Refresh nicht selbst das Fenster. Im gültigen Modul wird $65\le T\le80$ verlangt, um die Warnkante 64 auszuschließen. EWI setzt die isolierte Freigabe aus und füttert nicht. Ein externer Pull-down und ein gesperrter Startpfad tragen den Ausgang auch bei fehlendem Callback. Alle Beobachtungszellen bleiben ohne tatsächlichen Lauf leer.
