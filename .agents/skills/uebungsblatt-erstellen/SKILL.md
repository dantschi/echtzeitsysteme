---
name: uebungsblatt-erstellen
description: Erstellt oder überarbeitet Übungs- und Laborblätter mit überprüfter Musterlösung und getrennten PDFs; ergänzt bei Programmieraufgaben Startcode, Referenzlösung, Tests und ein geprüftes Downloadpaket.
---

# Übungsblatt erstellen

## Einsatz und Grundlagen

Verwenden, wenn Übungen, Programmieraufgaben oder Laborversuche zu einer Einheit erstellt oder angepasst werden.
Der Skill erzeugt ein bearbeitbares Blatt und dessen Musterlösung aus einer gemeinsamen Quelle.
Bei Programmieraufgaben kommen ein beabsichtigt unvollständiges Starterprojekt,
eine Referenzlösung und ein geprüftes Studierenden-ZIP hinzu.

Vor Beginn lesen:

- [Gemeinsame Arbeitsweise](../references/arbeitsweise.md) und [Projektregeln](../../../AGENTS.md)
- Einschlägige Abschnitte der [Fachregeln](../../../FACHREGELN.md)
- [Kurskontext](../../../README.md), [Kursglossar](../../../GLOSSAR.md), zugehörige Einheit und vorhandene Übungsblätter
- [Blattvorlage](../../../labs/lab_template.qmd), [Lab-Konfiguration](../../../labs/_quarto.yml)
  und [Lösungsprofil](../../../labs/_quarto-solution.yml)
- Für Einrichtung und Ausgaben die [technische Anleitung](../../../TEMPLATE.md)
- Bei Programmieraufgaben die [Code-Regeln](../../../code/labs/AGENTS.md) und
  die [Paketkonventionen](../../../TEMPLATE.md#programmieraufgaben-mit-startcode)

## Eingaben klären

Ermitteln: Thema/Einheit, Lernziele, Vorkenntnisse, Bearbeitungszeit, Einzel-/Gruppenarbeit und erlaubte Hilfsmittel.
Bei Software und Hardware benötigte Versionen, Plattform und tatsächliche Verfügbarkeit berücksichtigen.
Bei Startcode zusätzlich klären, welche Inhalte die Studierenden ergänzen sollen,
welche Schnittstellen vorgegeben sind und welcher anfängliche Build-/Testzustand erwartet wird.
Vorhandene Angaben zuerst aus dem Kurskontext und den Quellen übernehmen.
Nur entscheidende Lücken erfragen; didaktische Beispielwerte und andere tragfähige Annahmen kennzeichnen.

Bei Überarbeitungen bestehende Aufgabenfolge und Inhalte grundsätzlich erhalten.
Größere Änderungen der Schwierigkeit oder des didaktischen Aufbaus begründet vorschlagen.
Bereits beauftragte Änderungen direkt ausarbeiten; das Blatt nicht an einer Zwischenfreigabe stoppen.

## Ablauf

1. **Zuordnung herstellen.** Jede Aufgabe einem beobachtbaren Lernziel und dem erforderlichen eigenständigen Denkschritt zuordnen.
   Den Satz gemäß den [Didaktikregeln](../../../AGENTS.md#verständnis-transfer-und-prüfungsvorbereitung) abstufen:
   wenige Einstiege, bei passendem Lernziel Schwerpunkt auf selbstständiger Anwendung und Analyse, optional gekennzeichnete Vertiefung; keine starre Quote.
   Bearbeitung und Besprechung realistisch schätzen; Vorkenntnisse, Denk-/Rechenschritte, Hilfen und typische Fehlannahmen berücksichtigen.
2. **Kanonische Quelle anlegen.** Die Vorlage nach `labs/lab_NN_thema.qmd` übernehmen oder das bestehende Blatt bearbeiten.
   Eine freie Nummer passend zum Kurs verwenden; Blatt und PDF-Musterlösung bleiben in derselben Quarto-Quelle.
   Bei Programmieraufgaben gehört der Referenzcode zum Material und muss mit der PDF-Musterlösung übereinstimmen.
3. **Aufgaben formulieren.** Voraussetzungen, benötigte Hilfsmittel, Eingaben und erwartete Arbeitsergebnisse angeben.
   Relevante Randbedingungen, Darstellungen und Bewertungskriterien verständlich beschreiben.
   Aufgaben müssen mit den bereitgestellten Informationen lösbar sein.
   Für Transfer bekanntes Wissen auf eine neue Situation, einen Ablauf, eine Fehlerdiagnose, eine Begründung oder veränderte Annahmen anwenden lassen;
   bloße Folienparaphrasen und Zahlenwechsel als Wiederholung einordnen.
   Zur Prüfungsvorbereitung tatsächliche Prüfungsform und Anforderungen abgleichen, Hilfen schrittweise reduzieren und je nach Lernziel offene Rechen-, Begründungs- oder Codeaufgaben ergänzend zu Multiple Choice vorsehen.
4. **Versuche reproduzierbar beschreiben.** Bei Laboraufgaben Aufbau, Parameter, Arbeitsschritte und Auswertung angeben.
   Zulässige Daten und nötige Skripte im Repository erhalten; konkrete Hardware- und API-Aussagen belegen.
   Messungen, Simulationen und Modellrechnungen unterscheidbar benennen.
5. **Musterlösung vollständig ausarbeiten.** Rechenweg, Begründung, Ergebnis und typische Fehler erläutern.
   Lösungsinhalte im Aufgabenblatt ausschließlich in profilgebundene Bereiche schreiben:

   ```markdown
   ::: {.content-visible when-profile="solution"}
   **Musterlösung Aufgabe 1**

   Nachvollziehbarer Lösungsweg und Ergebnis.
   :::
   ```

   Auch lösungsverratende Hinweise oder Abbildungen entsprechend behandeln.
   Die Aufgabenstellung muss ohne Lösungsblöcke vollständig bleiben.
6. **Fachlich gegenprüfen.** Die Aufgaben aus Studierendensicht lösen und Angaben gegen Lösungen abgleichen.
   Am Lösungsweg mit Folien und ausgearbeiteten Beispielen vergleichen, was bereits vorgegeben ist und was selbstständig zu leisten bleibt.
   Die eingeführte Terminologie aus Folien und Lernnotizen in Aufgaben, Musterlösungen und Codeerklärungen beibehalten.
   Rechnungen unabhängig prüfen; Code mit relevanten Normal-/Randfällen prüfen, soweit Werkzeuge verfügbar sind.
   Fachabhängige Modellannahmen und Nachweisgrenzen offenlegen; keine Ausführung oder Messung erfinden.
7. **Beide Fassungen bauen.** Aus dem Repository-Stamm die gemeinsame Umgebung verwenden:

   ```bash
   source scripts/activate.sh
   python3 scripts/build-materials.py labs
   ```

   Fehlende Einrichtung anhand der technischen Anleitung herstellen; Renderfehler beheben.
8. **Lösungstrennung kontrollieren.** Beide frisch erzeugten PDFs öffnen und ihren Text/Inhalt vergleichen.
   Prüfen, dass Lösungen und Lösungskennzeichnungen nur in der Musterlösung erscheinen.
   Zusätzlich Aufgaben, Abbildungen, Formeln, Arbeitsraum und Seitenumbrüche auf Vollständigkeit und Lesbarkeit prüfen.
   Eine bloße Existenzprüfung oder Suche nach dem Wort „Musterlösung“ reicht nicht aus.
9. **Einbindung und Abschluss.** Bei Bedarf Kursübersicht auf die beiden vorgesehenen Ausgabepfade ergänzen.
   `python3 scripts/build-materials.py all` als vollständigen Projektcheck ausführen.
   Änderungen, tatsächliche Prüfungen und verbleibende Grenzen knapp berichten.

## Ergebnisse

- Eine nummerierte `.qmd`-Quelle mit vollständigen Aufgaben und profilgebundenen Musterlösungen
- Benötigte Daten, Diagrammquellen und Hilfsskripte für nachvollziehbare Bearbeitung
- Studierenden-PDF `_site/labs/lab_NN_thema.pdf`
- Lösungs-PDF `_site/labs/lab_NN_thema-musterloesung.pdf`
- Abschlussbericht einschließlich belegter Lösungstrennung und fachlicher Prüfungen

## Zusätzlicher Ablauf für Programmieraufgaben

1. Lege `code/labs/lab_NN_thema/` passend zur Blattkennung an. Lies bei vorhandenen
   Projekten zuerst README, Manifest und Code; erhalte Struktur und Schnittstellen.
2. Entwickle Aufgabenstellung, öffentliche Tests und Referenzlösung gemeinsam.
   Prüfe die Referenzlösung einschließlich relevanter Randfälle und leite daraus
   den Startcode mit klar zugeordneten TODOs wie `TODO: Aufgabe 1` ab.
   Der Startcode ist standardmäßig importierbar bzw. übersetzbar, aber fachlich noch unvollständig.
3. Schreibe eine Anleitung, die auch außerhalb des Checkouts im entpackten Paket
   funktioniert. Benenne Sprache/Version, Startbefehle, zu bearbeitende Dateien,
   Lernziele und erwartete anfängliche Testergebnisse.
4. Pflege `lab.json` nach den Paketkonventionen: Laufzeitprüfung, ausdrückliche
   `student_files` und `solution_files` sowie benannte Befehle mit getrennten
   Erwartungen für `starter` und `solution`. Nicht erfolgreiche Startertests
   benötigen zusätzlich zum Rückgabecode ein exakt passendes JSON-Testprotokoll.
   Ein unerwarteter Fehler darf nicht durch einen allgemeinen Fehlerstatus kaschiert werden.
5. Verlinke im Blatt dasselbe Startcode-ZIP wie in der Kursübersicht:
   `_site/labs/lab_NN_thema-startcode.zip` ist der öffentliche Build-Pfad.
   Aus einer `.qmd` direkt unter `labs/` lautet der relative Link `lab_NN_thema-startcode.zip`.
6. Baue mit `python3 scripts/build-materials.py labs` bzw. abschließend `all`.
   Diese Aufrufe prüfen Startcode und Referenzlösung und erzeugen das ZIP nur aus
   ausdrücklich freigegebenen Dateien. Der tatsächlich entpackte Download wird
   nochmals geprüft. TODOs im Startcode dabei nicht automatisch vervollständigen.
7. Vergleiche Aufgabenblatt, Code, Tests und PDF-Musterlösung; kontrolliere das
   Studierendenpaket auf Vollständigkeit und versehentlich enthaltene Lösungen.
   Berichte erwartete Starterfehlschläge getrennt von unerwarteten Defekten.

Das ZIP enthält keine Referenzlösung. In einem öffentlichen Repository bleibt
der Ordner `solution/` dennoch öffentlich; eine gewünschte spätere Freigabe muss
über eine geeignete getrennte Ablage umgesetzt werden.

## Abschlussprüfung

- Sind jede Aufgabe und ihre Lösung unter denselben dokumentierten Annahmen korrekt und vollständig?
- Passen Schwierigkeit, Hilfsmittel und Umfang zum Kurs und zum Zeitbudget?
- Ist der eigenständige Denkschritt je Aufgabe belegt und das geschätzte Niveau am verfügbaren Prüfungsbezug ausgerichtet?
- Sind beide PDFs frisch, lesbar und auf Lösungstrennung geprüft?
- Sind tatsächlich ausgeführte Versuche und lediglich hergeleitete Ergebnisse klar unterschieden?
- Bei Programmieraufgaben: Passen TODOs und Tests zum Blatt, ist die Referenzlösung geprüft,
  und funktioniert das entpackte ZIP mit dem dokumentierten erwarteten Starterzustand?

## Beispielaufträge

- „Erstellen Sie Blatt 3 zu Zweierkomplement und Überlauf mit 45 Minuten Bearbeitungszeit.“
- „Ergänzen Sie das Interrupt-Labor um eine Aufgabe zu gemeinsam genutzten Daten samt Musterlösung.“
- „Erstellen Sie eine Python-Aufgabe zur linearen Suche mit Startcode, öffentlichen Tests und Musterlösung.“
