# Quizfragen für quizacademy

Diese Regeln ergänzen die [zentralen Projektregeln](../AGENTS.md).

Gilt beim Erstellen und Überarbeiten von CSV-Dateien in `quizacademy/` (`quizacademy/**/*.csv`). Zielgruppe: Bachelor EIT, DHBW Stuttgart, mit den im Kurskontext dokumentierten Vorkenntnissen. Fragen, Antwortoptionen und Erläuterungen müssen auf den behandelten Konzepten und Lernzielen der zugehörigen Vorlesung beruhen. Falsche Optionen dürfen gezielt falsche Kombinationen oder Verallgemeinerungen dieser Konzepte enthalten; unbekanntes Zusatzwissen darf nicht über die Lösung entscheiden. Keine Messwerte, APIs oder Hardwareläufe erfinden. Rechenfragen mit ausdrücklich angegebenen didaktischen Ausgangsdaten sind erlaubt; Ergebnisse müssen rechnerisch geprüft sein.

## Interaktiver Einsatz im EZS-Kurs

Für das interaktive Quiz stehen pro Frage 25 Sekunden einschließlich Lesen und Auswahl zur Verfügung. Halte Fragen und alle vier Optionen gemeinsam kurz und vermeide mehrstufige Zeitrechnungen. Jede Frage muss ohne Kenntnis eines konkreten Folienfalls verständlich sein und alle für die Lösung erforderlichen Angaben enthalten. Die Zeitvorgabe begrenzt den Umfang, ersetzt aber keine fachliche Verständniskontrolle.

## Syntax nicht ändern

Semikolon, eine Kopfzeile, genau vier Optionen, genau eine richtige Antwort:

```text
Frage;Typ;Antwort 1;Korrekt 1;Antwort 2;Korrekt 2;Antwort 3;Korrekt 3;Antwort 4;Korrekt 4;Erläuterung
```

`Typ` bleibt `Multiple Choice`. Korrektheit nur `correct` oder `incorrect`. Kein Semikolon in Frage, Antwort oder Erläuterung. Spalten weder ergänzen noch umbenennen. Bestehende Dateien behalten ihre Fragenzahl, sofern nicht ausdrücklich anders verlangt.

## Schwierigkeit

Wende die zentralen Regeln zu [Verständnis, Transfer und Prüfungsvorbereitung](../AGENTS.md#verständnis-transfer-und-prüfungsvorbereitung) an. Lege für jede Frage Lernziel und erforderliche eigenständige Denkleistung fest. Begriffsfragen bleiben für grundlegende Lernziele zulässig. Prüfe bei Anwendungs- und Transferzielen, ob die Frage mehr als das Wiederfinden einer Folienaussage verlangt: Auch eine Paraphrase oder ein bloßer Zahlen- bzw. Synonymwechsel kann dieselbe Routine abfragen.

Nutze dafür neue, mit dem eingeführten Wissen lösbare Situationen, etwa einen Ablauf herleiten, einen Fehler diagnostizieren, eine geänderte Bedingung beurteilen oder eine Begründung auswählen. Mögliche Unterscheidungen sind notwendige gegen hinreichende Aussage, zwei ähnlich benannte Größen oder erlaubter Sonderfall gegen verbotene Verallgemeinerung. Behalte die eingeführte Terminologie gemäß [Kursglossar](../GLOSSAR.md) bei; neue Benennungen erzeugen keinen fachlichen Transfer. Falsche Optionen bilden typische Fehlvorstellungen aus derselben Vorlesung ab, keine absurden Außenseiter.

Plane im Fragensatz wenige passende Einstiegsfragen und einen Schwerpunkt auf selbstständiger Anwendung und Analyse, soweit dies den Lernzielen entspricht. Kennzeichne gegebenenfalls weiterführende Vertiefungen. Verwende keine starre Quote; erhalte bei Überarbeitungen die beauftragte Struktur und Fragenzahl.

Schätze den Anspruch anhand von Vorwissen, Denk- und Rechenschritten, verfügbaren Hilfen und Bearbeitungszeit begründet ein. Zusätzliche Rechenlast, komplizierte Formulierungen und Fangfragen ersetzen keine fachliche Denkleistung. Nutze vorhandene studentische Rückmeldungen und Bearbeitungszeiten zur Kalibrierung; ohne solche Daten bleibt die Schwierigkeit eine begründete Einschätzung.

Beziehe die Prüfungsvorbereitung auf tatsächlich vorliegende Vorgaben und Aufgabenbeispiele; erfinde weder Prüfungsanforderungen noch eine garantierte Prüfungsnähe. Multiple-Choice-Fragen können Verständnis und Lösungsentscheidungen prüfen, ersetzen aber keine offenen Rechen-, Begründungs- oder Programmieraufgaben, wenn deren selbstständige Bearbeitung zum Lernziel gehört.

Formuliere die nötigen Voraussetzungen so, dass unter ihnen genau eine Option richtig ist. Die Erläuterung begründet in einem oder zwei Sätzen die richtige Antwort und die maßgebliche Abgrenzung zu den falschen Optionen. Halte Fragestellungen eindeutig und vermeide unnötige Verneinungen und sprachliche Fangfragen.

Prüfe fachabhängige Annahmen nach den [einschlägigen Fachregeln](../FACHREGELN.md). Nenne insbesondere Wortbreite, Vorzeicheninterpretation, Anfangszustand oder Zeitmodell, wenn sie über die richtige Antwort entscheiden und nicht bereits im Kurskontext eindeutig festgelegt sind. Code mit undefiniertem Verhalten darf keine vermeintlich eindeutig vorhersagbare Ausgabe als richtige Antwort voraussetzen.

## Antwortgestaltung

Richtige Antworten dürfen nicht systematisch durch Länge, Detailgrad, Grammatik oder wiederkehrende Zusätze erkennbar sein. Formuliere alle Optionen als vergleichbar präzise, in sich abgeschlossene Aussagen. Nutze keine feste Längenquote und verlängere falsche Optionen nicht künstlich. Verteile die Positionen der richtigen Antworten ohne auffälliges Muster.

## Qualitätsprüfung

Prüfe vor dem Abschluss die unveränderte Kopfzeile, elf Spalten je Zeile, `Multiple Choice`, vier nichtleere und unterschiedliche Antwortoptionen sowie genau ein `correct` und drei `incorrect`. Prüfe außerdem fachliche Eindeutigkeit, Bezug zu Vorlesung und Lernziel, Rechenergebnisse und unbeabsichtigte Lösungshinweise. Prüfe ausdrücklich die erforderliche Denkleistung und den Abstand zum vorgerechneten bzw. erklärten Folienbeispiel sowie Anspruch und Abdeckung des gesamten Fragensatzes.

Halte für Lehrende je Frage knapp Lernziel, Denkschritt und begründeten Anspruch fest; bei Anwendungs- und Transferfragen auch, was gegenüber der Vorlage selbstständig übertragen werden muss. Eine kompakte Prüfnotiz im Arbeitsbericht genügt; keine zusätzlichen CSV-Spalten oder gesonderten Dateien erzwingen. Berichte nur Prüfungen, die tatsächlich durchgeführt wurden.
