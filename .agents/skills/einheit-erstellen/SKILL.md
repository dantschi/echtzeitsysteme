---
name: einheit-erstellen
description: Erstellt oder überarbeitet eine Vorlesungseinheit als Quarto-Folien mit vollständigen Lernnotizen, Beispielen und geprüften Ausgaben.
---

# Vorlesungseinheit erstellen

## Einsatz und Grundlagen

Verwenden, wenn eine konkrete Einheit neu entsteht, ergänzt oder inhaltlich überarbeitet wird.
Für die Planung eines ganzen Moduls [kurs-planen](../kurs-planen/SKILL.md) verwenden.
Ein reiner Review gehört zu [material-review](../material-review/SKILL.md),
eine reine Ausgabeprüfung zu [material-validieren](../material-validieren/SKILL.md).

Vor Beginn lesen:

- [Gemeinsame Arbeitsweise](../references/arbeitsweise.md)
- [Projektregeln](../../../AGENTS.md) und [Folienregeln](../../../vorlesungen/AGENTS.md)
- Einschlägige Abschnitte der [Fachregeln](../../../FACHREGELN.md)
- [Kurskontext](../../../README.md), [Projektkonfiguration](../../../_quarto.yml) und betroffene Quellen
- Bei Build-Fragen die [technische Anleitung](../../../TEMPLATE.md)

## Eingaben klären

Ermitteln: Thema, Einheit/Dateipfad, Lernziele, Zeitbudget, Vorkenntnisse und Anschluss an vorherige Einheiten.
Sprache/Standard, Zielplattform, Werkzeuge und Zeitmodell nur soweit für die Inhalte erforderlich klären.
Vorhandene Angaben aus Kurskontext und Nachbareinheiten nutzen; Quellen und bereitgestellte Materialien prüfen.
Nur fehlende Angaben erfragen, die den Entwurf wesentlich bestimmen; sonst begründete Annahmen offen benennen.

Bei einer Überarbeitung vorhandene Gliederung, Inhalte und Dateinamen standardmäßig erhalten.
Größere didaktische Umstrukturierungen mit Grund und erwarteter Wirkung vorschlagen.
Bereits ausdrücklich beauftragte Umstrukturierungen direkt umsetzen; keine erneute Freigabe verlangen.
Bei Entwurf oder inhaltlicher Überarbeitung die Einstiegsfrage gemäß den Folienregeln umsetzen.
Eine reine Ausgabeprüfung oder kleine technische Änderung strukturiert eine bestehende Einheit nicht automatisch um.

## Ablauf

1. **Bestand aufnehmen.** Zielquelle, Nachbareinheiten, vorhandene Beispiele und Diagrammquellen lesen.
   Das [Kursglossar](../../../GLOSSAR.md) mit zugehörigen Übungen, Lösungen und Quizfragen abgleichen;
   eingeführte Begriffe auch in Folientext, Lernnotizen und Codeerklärungen konsistent weiterführen.
   Bestehende Nutzeränderungen beachten und den konkret beauftragten Umfang abgrenzen.
2. **Lernweg festlegen.** Wenige beobachtbare Lernziele mit einer passenden Übung oder Verständnisprüfung verbinden.
   Als erste inhaltliche Folie nach dem Titel und vor Agenda/Lernzielen eine [aktivierende Einstiegsfrage](../../../vorlesungen/AGENTS.md#aktivierende-einstiegsfrage-je-vorlesung) vorsehen,
   die mit dokumentiertem Vorwissen oder Alltagserfahrung eine begründete Vermutung, Vorhersage oder Entscheidung ermöglicht.
   Denk-/Austauschphase und das Sammeln begründeter Antworten im Zeitbudget berücksichtigen; die Antwort auf der Einstiegsfolie offenlassen.
   Je Aufgabe den eigenständigen Denkschritt bestimmen und den Aufgabensatz nach den zentralen [Didaktikregeln](../../../AGENTS.md#verständnis-transfer-und-prüfungsvorbereitung) abstufen.
   Zeit für Erklärung, aktive Bearbeitung, Fragen und Besprechung einplanen; den roten Faden fortführen.
   Dies als Arbeitsgrundlage nutzen und die beauftragte Einheit anschließend vollständig ausarbeiten.
3. **Quarto-Quelle bearbeiten.** Neue Einheiten passend zur vorhandenen Nummerierung unter `vorlesungen/` anlegen.
   Bestehende Frontmatter-Konventionen übernehmen; jede Folie im Dokumentkörper mit `## Titel` beginnen.
   `---` im Dokumentkörper nicht als Folientrenner verwenden.
4. **Folien und Notizen gemeinsam schreiben.** Jede Folie erhält vollständigen Lerntext in `::: notes`.
   Zur Einstiegsfrage erwartbare Antworten, Fehlvorstellungen, Annahmen, fachliche Auflösung und Übergang in den Notizen ausarbeiten;
   die Frage im Verlauf oder Abschluss wieder aufgreifen und beantworten.
   Auch Orientierung, Übungen und Abschluss benötigen Notizen; Übungslösungen darin nachvollziehbar ausarbeiten.
   Neue Begriffe, Code und Abbildungen erklären und Voraussetzungen, Grenzen und Übergänge sichtbar machen.
5. **Beispiele prüfen.** Eingaben, Anfangszustände und Modellannahmen festlegen.
   Rechenwege und relevante Normal-/Randfälle unabhängig nachvollziehen; passende Fachprüfungen anwenden.
   Übungslösungen mit Folien und erklärten Beispielen vergleichen: Welche Anwendung oder Begründung müssen Studierende noch selbst leisten?
   Nur tatsächlich ausgeführte Programme oder Versuche als ausgeführt beschreiben.
6. **Darstellungen ergänzen.** Den Diagramm-Entscheidungsbaum aus den Projektregeln anwenden.
   Bearbeitbare Quellen und nötige Erzeugungsskripte erhalten; Aussagen mit Folientext und Notizen abgleichen.
   Quantitative Zeitdiagramme und Kurven aus dem Modell ableiten und fachlich prüfen.
7. **Einbindung pflegen.** Kursübersicht und Verweise bei Bedarf um die konkrete Einheit ergänzen.
   Bestehende Theme-, Profil- und Extension-Konventionen beibehalten.
8. **Rendern und korrigieren.** Aus dem Repository-Stamm die gemeinsame Umgebung aktivieren:

   ```bash
   source scripts/activate.sh
   python3 scripts/build-materials.py website
   python3 scripts/build-materials.py handouts
   ```

   Fehlendes Setup anhand der technischen Anleitung herstellen; keine unabhängigen Werkzeugversionen installieren.
   HTML und reguläres PDF-Handout auf Lesbarkeit, Überlauf und vollständige Notizzuordnung prüfen.
   Erkannte Mängel beheben und betroffene Ausgaben erneut erzeugen.
9. **Abschließen.** Den vollständigen Projektcheck `python3 scripts/build-materials.py all` ausführen.
   Tatsächliche Prüfungen und verbleibende Grenzen knapp berichten; keine alten Quellordner-PDFs als Nachweis nutzen.

## Ergebnisse

- Vollständige `.qmd`-Einheit mit Lernzielen, Übungen, Lösungen in Notizen und belegten Aussagen
- Benötigte Diagrammquellen, Abbildungen und reproduzierbare Hilfsskripte
- Gegebenenfalls aktualisierte Einträge in der Kursübersicht
- Frische Folien unter `_site/vorlesungen/` und reguläres PDF-Handout mit Suffix `-handout.pdf`
- Kurzer Abschlussbericht mit Änderungen, Annahmen, ausgeführten Prüfungen und offenen Grenzen

## Abschlussprüfung

- Decken Inhalte und Übungen die Lernziele im Zeitbudget ab?
- Hat jede Folie verständliche, vollständige und fachlich konsistente Notizen?
- Ist die Einstiegsfrage passend zum Vorwissen platziert, in den Notizen vollständig ausgearbeitet und später beantwortet?
- Stimmen Aussagen, Code, Rechnungen und Diagramme unter den dokumentierten Annahmen überein?
- Sind UTF-8 ohne BOM, Formeln, Kontraste, Beschriftungen und Verweise geprüft?
- Sind Folien und Handout frisch erzeugt und visuell geprüft oder konkrete Prüfgrenzen offengelegt?

## Beispielaufträge

- „Erstellen Sie Einheit 4 zu synchronen Automaten für 90 Minuten auf Basis unseres Kursrahmens.“
- „Überarbeiten Sie die Timer-Einheit; erhalten Sie die Struktur und ergänzen Sie ein nachvollziehbares Rechenbeispiel.“
