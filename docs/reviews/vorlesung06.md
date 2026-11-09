# Review und Korrekturen: Vorlesung 6

Review vom 9. November 2026 nach dem Skill `material-review`, Ausgangsstand
`a99e288`. Auftrag: fachliche und didaktische Prüfung samt direkter Korrektur,
anschließend Commit und Push.

## Umfang und Maßstab

Geprüft wurden alle Folien und Notizen von
`vorlesungen/06_server-verfahren.qmd`, die sechs Zeitdiagramme,
`labs/lab_06_server-verfahren.qmd` einschließlich Musterlösung,
`labs/vertiefung/lab_06_programmierung.md`, `code/lab06/README.md` und
`quizacademy/quiz_06_server-verfahren.csv`. Kursrahmen, Vorwissen aus Einheiten
1–5, Papierklausur, Glossar und 25 Sekunden Antwortzeit im Quiz sind berücksichtigt.

## Befunde und umgesetzte Korrekturen

Die Fundstellen beziehen sich auf den Ausgangsstand `a99e288`.

| Priorität | Fundstelle | Problem und Wirkung | Eingearbeitete Korrektur |
|---|---|---|---|
| P1 | Quiz, Zeilen 2–11, insbesondere Zeile 7 | 57–95 Wörter einschließlich Antworten und zusätzlich eine Fixpunktrechnung überfrachten 25 Sekunden. | Zehn eigenständige Fragen mit 25–46 Wörtern; die RTA-Frage prüft jetzt den Geltungsbereich des gegebenen Nachweises. |
| P2 | Vorlesung, Zeile 26 | Der Einstieg zeigt eine Vorwissenstabelle, aktiviert aber keine eigene Vermutung. | Neue erste Inhaltsfolie zur größeren Diagnosequeue und Reglerfrist, kurze Denk-/Austauschphase, vollständige Auflösung in den Notizen und Wiederaufnahme im Klausurcheck. |
| P2 | Vorlesung, Zeilen 1045–1055; Vorbereitungsleitfaden, Zeile 27 | Bedienungsschritte und Endzustand beantworten die selbst zu erarbeitende Praxisprognose bereits. | Sichtbare Folie und Leitfaden fordern die Prognose ein; die Lösung bleibt in den Vortragnotizen bzw. im Lösungsprofil. |
| P2 | Übung, Aufgabe 6.2, ursprünglich Zeile 140 | Die geforderte Handleprüfung fehlt in der Musterlösung. Ein zurückkehrender Error_Handler würde die Fehlerpfade nicht sicher stoppen. | Handleprüfung vor Kernelstart und nicht zurückkehrende Fehlerpfade ergänzt; keine zweite Thread- oder Kernelanlage. |
| P2 | Übung, Aufgaben 2 und 6 | Die Ablaufwiederholung allein verlangt wenig Übertragung. | Aufgabe 2 ergänzt eine begründete Budgetdimensionierung; Aufgabe 6 enthält eine Papiervariante mit früherer erster Ankunft und zweimaligem Restbudgetverfall. |
| P2 | Quiz, Zeilen 4–6 und 10–11 | Auffüllphase, Annahmen des SS-Spezialfalls, Ticksemantik und Voraussetzungen einer exakten Ratenrechnung sind nicht überall eindeutig. | Budget/Reset, keine Zwischenauffüllung bzw. Höherstörer, CMSIS-Kernelticks und gegebene Bedienrate explizit benannt. |
| P3 | Übung, Zeilen 114/124 | Aufgaben 5 und 6 haben keine Punkte. | Vier bzw. zwölf Punkte mit Teilpunkten ergänzt; zusammen 48 Punkte, davon 36 für Papieraufgaben. |
| P3 | Praxisfolien und Zeitplan | Endzustand vor Reset ist mehrdeutig, vier gefragte Felder werden in den Notizen nur teilweise benannt. Praxiszeit und Aufgabenpool sind nicht klar zugeordnet. | Endzustand t=18 vor dem nächsten Zyklus eindeutig; 180-Minuten-Plan mit 45 Minuten Papierauswahl und 30 Minuten eigener Implementierung. |
| P3 | Zeitdiagramme und Glossar | Helle tragende Konturen und nur ein allgemeiner Serverbudget-Eintrag erschweren die Orientierung. | Tragende Konturen kontrastreicher; Background, Polling, Deferrable und Sporadic mit ihren Grenzen ins Glossar aufgenommen. |

Die bestehenden Zahlenbeispiele und ihre eingeschränkten klassischen
Sporadic-Server-Annahmen sind fachlich konsistent. Die Regel „Verbrauch zum
Abschnittsbeginn plus Zeitraum zurückgeben“ wird weiterhin ausdrücklich nur
für ununterbrochene Abschnitte ohne höher priorisierte Arbeit und ohne weitere
Auffüllung im Abschnitt verwendet. Eine vollständige allgemeine SS-Implementierung
wird daraus nicht abgeleitet.

## Lernziele und eigenständige Arbeit

| Aufgabe | Lernziel / Denkschritt | Einordnung |
|---|---|---|
| Papier 1 | Eigene CPU-Zeit und verstrichene Zeit trennen, RTA durchführen und ihre Aussage begrenzen | Gestützte Wiederholung und Anwendung |
| Papier 2 | FIFO-Ablauf herleiten und kleinstes ganzzahliges Budget für eine geänderte Frist bestimmen | Ablaufanalyse und Dimensionierung |
| Papier 3 | DS und den angegebenen klassischen SS-Fall vergleichen | Verfahrensanwendung und Begründung |
| Papier 4 | Bedienrate, Pufferwachstum und Grenzen einer Nachrichtenbatch-Schleife beurteilen | Rechnung und Implementierungsdiagnose |
| Papier 5 | Budgetpause und zusätzliche Voraussetzungen einer Antwortzeitzusage erklären | Kurze Begriffs- und Vertragsprüfung |
| Programmierung 6 | Einen endlichen Polling-Modellschritt selbst implementieren, vorher prognostizieren und einen Ankunftsrandfall analysieren | Eigene C-Implementierung mit Papierübertragung |

Für Papieraufgaben 1–5 sind etwa 40–50 Minuten, für die Programmieraufgabe
25–35 Minuten geschätzt. Die Lehrperson wählt für den 45-Minuten-Papierblock
zwischen Blatt- und Folienaufgaben. Aussagen zur tatsächlichen Bearbeitungszeit
und Schwierigkeit sind ohne studentische Bearbeitungsdaten keine empirische
Kalibrierung.

Quiz-Lernziele in Reihenfolge der zehn Fragen: eigene CPU-Zeit zählen,
verfallene Polling-Gelegenheit erkennen, DS-Budgets über die Grenze verbinden,
SS-Rückgabe aus dem Abschnittsbeginn bestimmen, Restarbeit und Budget trennen,
Servernachweis von Anfrageantwortzeit unterscheiden, unveränderten Quotienten
bei proportionaler Skalierung erkennen, Budgetpause beurteilen, Nachrichtenanzahl
von CPU-Reservation trennen und Dauerüberlast gegenüber zusätzlichem Speicher
beurteilen. Die kurzen Regelanwendungen werden nicht als anspruchsvoller Transfer
ausgegeben. Mehrstufige Rechnungen bleiben in den Papieraufgaben.

## Geprüfte Rechnungen und Grenzen

- Hauptvergleich: Polling-Abschlüsse 9/10/18, Deferrable 2/7/11,
  Sporadic 2/7/15 ms; im SS Rückgaben bei 9/14/17/22 ms mit je einer Einheit.
- Zweites SS-Beispiel: Abschlüsse 3/9/19 ms, Rückgaben (12,1), (17,2),
  (22,1), (27,2).
- RTA-Hauptbeispiel: 1/3/6 ms, Auslastung 5/8. Papierfolie 3: 1/3/5 ms,
  Auslastung 1/2. DS-Gegenbeispiel: naive RTA 11 ms gegenüber tatsächlich
  13 ms Antwortzeit, Abschluss 19 bei Freigabe 6.
- Übungsblatt: Auslastung 15/28, Serverantwort 4 ms und Zustandsmeldung 7 ms.
  Polling-Abschlüsse 13/14/25/26 ms. Für den neuen Abschlussvertrag genügt
  mindestens Budget 5 ms; Budget 4 verschiebt die letzte Anfrage auf 25 ms.
- C-Modell: originale Bedienungsschritte 6/7/12/13, Papiervariante
  0/6/12/13. Endzustand jeweils t=18, pending=0, budget=0, served=4.
- Unabhängige diskrete Simulation und Fixpunktrechnung wurden gegen die
  Hauptbeispiele und Diagrammspuren geprüft. Die gedruckten C-Bausteine wurden
  mit GCC 14.2 unter C11 und strengen Warnungen mit HAL-/CMSIS-Stubs übersetzt;
  beide Modellfolgen, Bilanzinvarianten, Reset und gültiger Handlepfad bestanden.

Dies ist kein ARM-Firmwarebuild und kein Boardnachweis. Der LED-Versuch bleibt
eine verlangsamte Simulation mit Modellzeit, keine echte CPU-Reservation.
Eine komplette serverseitige Aktivierungs-/Auffüllimplementierung und ein neuer
externer Literaturabgleich sind nicht Bestandteil dieser Korrekturen.

## Ausgabeprüfung und Abschluss

- Vollständiger Materialbuild mit der versionsgebundenen Toolchain erfolgreich:
  Startseite und neun Vorträge, neun Handouts sowie zehn Aufgabenblätter in
  beiden Profilen. Danach Website für die finalen Notizkorrekturen und das
  VL6-Handout gezielt erneut frisch gerendert.
- VL6: 50 Folien und 50 Notizblöcke, im Handout jede Foliennummer vorhanden.
  Alle 50 Vortragsfolien im Browser auf sichtbare Überläufe geprüft.
  Interne unsichtbare MathJax-Messspannen in der Überschrift von Folie 9
  wurden von tatsächlichem Folieninhalt unterschieden und gesondert überprüft.
  MathJax 2.7.9 kam für den Browserlauf aus dem offiziellen npm-Paket im
  lokalen Testcache; die Auslieferungs-URLs bleiben unverändert.
- Studentenversion der Übung: vier Seiten, Musterlösung: sieben Seiten.
  Alle sechs Aufgaben in beiden Fassungen enthalten, Lösungstext und
  vollständiger Programmcode ausschließlich im Lösungsprofil.
- Sichtproben: Browserfolien 2, 5, 9 und 47; PDF-Handout Seiten 1 und 18
  sowie Zeitplan; Übung, Studentenseite 4 und Lösungsseite 7. Die automatisierte
  Folienprüfung ersetzt keine Sichtprüfung sämtlicher PDF-Seiten.
- Quiz-Formatvalidator erfolgreich: zehn Fragen, elf Spalten, vier verschiedene
  Optionen und jeweils eine richtige Antwort. Frage samt Optionen umfasst
  25–46 Wörter. Schwierigkeit und 25-Sekunden-Eignung sind begründete Schätzungen.
- Modelle, Fixpunktrechnungen, Budgetdimensionierung, beide C-Modellfolgen,
  Teilpunktesummen (48) und UTF-8 geprüft; kein Boardlauf behauptet.
