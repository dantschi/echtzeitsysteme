# Freiwillige Programmiervertiefung zu Übungsblatt 9

Diese Anleitung ist für die **Vorbereitung durch die Lehrperson** oder eine **freiwillige Vertiefung außerhalb des Übungstermins** gedacht. Sie gehört nicht zum verbindlichen Papierkern. Die Zeitangaben der folgenden Abschnitte gelten allenfalls für einen bereits vollständig vorbereiteten, funktionierenden Projektstand; sie sind keine Zusage für Erstintegration und Fehlersuche.

Verwenden Sie STM32CubeMX und VS Code mit STM32 VS Code Extension für das NUCLEO-H755ZI-Q / STM32H755ZIT6. Die Quellen in `code/lab09/` sind Anwendungsbausteine; ein vollständig erzeugtes, auf dem Board geprüftes Firmwareprojekt wird hier nicht bereitgestellt. Versionen, Startfolge und tatsächliche Integration müssen vor der Demonstration geprüft werden.

Im Übungstermin wird höchstens eine kurze vorbereitete Demonstration gezeigt. Bei einem technischen Problem wechseln Sie spätestens nach drei Minuten zum vollständig angegebenen Papierfall im [Übungsblatt](../lab_09_capstone-timing-verifikation.qmd). Studierende müssen frühere Programmiervertiefungen nicht erfolgreich abgeschlossen haben, um dem nächsten Übungsblatt folgen zu können.

Wer freiwillig selbst integriert, dokumentiert nur den tatsächlich erreichten Stand. Modellwerte werden nicht als Boardmesswerte eingetragen. Die folgenden technischen Erläuterungen sind offen lesbar und unterstützen die Vorbereitung; die klausurnahen Musterlösungen bleiben im Profil `solution` des Übungsblatts.

## Labor · Zwei endliche Läufe · 25 Minuten

Verwenden Sie **STM32CubeMX und VS Code mit STM32 VS Code Extension für das NUCLEO-H755ZI-Q**. Grundlage ist ein vorbereitetes gemeinsam startendes Dual-Core-CMake-Projekt. Die vollständigen Anwendungsdateien und die Integration stehen in [code/lab09/README.md](../../code/lab09/README.md). Der Datenkern startet **keinen Watchdog**; C7 bleibt hier Entwurfs-/Rechenprüfung. Eine WWDG-Station aus Vorlesung 8 ist ein separater Versuch.

1. Beide Ziele: `lab09_capstone.h`, `lab09_shared.c`; M7 zusätzlich `lab09_cm7.c` und `lab09_output.c`; M4 zusätzlich `lab09_cm4.c` und `lab09_timer.h/.c`. Pro Ziel genau `CORE_CM7` oder `CORE_CM4`. Entfernen Sie vorherige Laborobjekte, die denselben Shared-Bereich nutzen.
2. Der neue Block hat 44 Byte Nutzstruktur und 64 Byte Section bei 32-Byte-Ausrichtung. In beiden echten Linkerskripten SRAM4 ab `0x38000000`, 64 KiB, exklusiv reservieren und `shared-section.ld` integrieren. Private Daten und Stacks bleiben außerhalb. Prüfen Sie beide ELF-/Map-Dateien und Starttabellen.
3. M7: bei ausgeschaltetem Datencache eine tatsächlich freie MPU-Region mit passenden nicht cachebaren Attributen konfigurieren, `Lab09_CM7_InitShared(LAB09_NORMAL)` ausführen, während M4 noch im vorhandenen Boot-Stop wartet. Danach Cache an, bestehende Bootfreigabe, einmal `Lab09_CM7_Run(1u)`. Kein Live-Initialisieren bei laufendem Partner.
4. M4: TIM2 und `htim2` vor dem Worker für **100 Hz** initialisieren, noch nicht starten. Verifizieren Sie den wirksamen Timertakt, Vorteiler und Periodenregister sowie die erlaubte NVIC-Priorität. Ergänzen Sie einmal `Lab09_CM4_TimerCallback(htim)` im vorhandenen HAL-Timercallback und erhalten Sie die Behandlung der HAL-Zeitbasis anderer Timer. Der einzige 1-kHz-CMSIS-RTOS2-Worker ruft `Lab09_CM4_Run(argument)` auf. Er erzeugt die Queue vor der Quellenfreigabe und beendet nach 20 Softwareeingaben die Quelle und Verarbeitung.
5. Beide Images bauen und programmieren, gemeinsam neu starten. Erst nach **beiden** `lab09_complete=1` und Fehlercodes null halten Sie beide Kerne an. Lesen Sie M4-Statistik, gemeinsame Mailbox und M7-Filterergebnisse. Berücksichtigen Sie die Debug-/Cachesicht privater M7-Daten gemäß Integrationsanleitung. Ein Timeout ist keine erfolgreiche Abnahme.
6. Sättigung: M7-Initialisierung auf `LAB09_SATURATION` ändern, beide Images neu bauen/programmieren und gemeinsam starten. Der M4 erzeugt zehn Puts aus der Task vor Get, ohne Timerstart. Prüfen Sie acht Annahmen und zwei Drops sowie die Mailboxbilanz. Das ist kein Test physischer Interruptlatenzen.
7. Falls vorbereitet: normaler Lauf mit `Lab09_CM7_Run(25u)` oder aktivierter begrenzter UART-Ausgabe. Erwarten Sie mögliche zusätzliche Ersetzungen, keinen festen Snapshot-Sollwert. Der Filter berechnet auf der tatsächlich kopierten Folge. Ausgabe bleibt außerhalb des HSEM.

| Konfiguration | Ihre Angabe |
|---|---|
| Boardrevision, CubeMX, HAL, Extension, Kerneladapter | |
| Kern-/Bustakte, TIM2-Takt, PSC/ARR, Kerneltick | |
| Boot-/Nutz-HSEM, MPU-Region, Shared-Adressen und Section | |
| beide CMake-Ziele, Starttabellen, Heap/Stack geprüft | |

| Lauf | $I,A,D_q,E_{put}$ | $Q,F,P,D_{task}$ | $S,O,U$ | Fehler / Erklärung |
|---|---|---|---|---|
| normal | | | | |
| Sättigung | | | | |
| langsam / UART optional | | | | |


**Technische Erläuterung Labor**

Normallauf: I=20. Ohne andere Abweisungen oder Verwürfe gilt A=P=20, aber S kann kleiner sein; O erklärt die übrigen Publikationen. Finale U=0, weil M7 den letzten Stand kopiert. Sättigung: I=10, A=8, Dq=2, Eput=0, ohne Taskverwurf P=8. Finale Q=F=U=0, also P=S+O. Die genaue Snapshotzahl hängt vom tatsächlichen Ablauf ab. Filterergebnisse müssen zur aufgezeichneten kopierten Folge passen, nicht zu erfundenen ausgelassenen Werten. Keine Beobachtungszelle wird ohne Lauf ausgefüllt.

## Abnahmeblatt und Aussagegrenzen

Für jedes Kriterium werden Bedingungen, Beleg und Status getrennt notiert. Zeitziele brauchen eine geeignete Aufnahme und gemeinsame beziehungsweise lokal passende Zeitbasis. `event_tick` ist der lokale M4-HAL-Millisekundenwert der Ereignisroutine; er wird nicht von einer unkorrelierten M7-Uhr subtrahiert und löst 50 µs nicht auf. Kopierte neue Stände tragen dieselbe Ereignis- und Publikationskennung wie die zugehörige M4-Publikation. Eine Aufnahme muss auch Auslassungen und fehlende Paare nennen.

| Kriterium | Ziel / Umfang | Beleg und Status |
|---|---|---|
| C1 | Last und Takte: normal 20 Eingaben, nominal 100 Hz | |
| C2 | kontrolliert 8 Annahmen, 2 Queue-Drops | |
| C3 | normale Publikationsantwort höchstens 1 ms | |
| C4 | neu kopierter Stand höchstens 50 ms alt | |
| C5 | Besitz, Layout, Start, Sichtbarkeit | |
| C6 | lokale Haltezeit je Seite höchstens 50 µs | |
| C7 | Watchdogrechnung und Fortschrittsentwurf | |
| C8 | Eingang-/Worker- und Mailboxbilanzen | |

Zulässige Statuswerte: **nicht geprüft**, **bestanden unter genannten Bedingungen**, **verletzt**, **Messung ungültig**. Ohne gemeinsame Aufnahme bleibt C4 nicht geprüft. Ein Quellreview oder Hosttest ist kein Boardlauf. Ein beobachtetes Maximum beweist keine allgemeine obere Antwortzeit oder WCET, Worst-Case Execution Time. Abgabe: vier Papierlösungen, Konfigurationsblatt, echte Beobachtungswerte, Bilanzprüfung und Kriterienstatus.
