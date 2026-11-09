---
name: material-validieren
description: Geänderte Lehrmaterialien dieses DHBW-Quarto-Templates mit der versionsgebundenen Umgebung bauen, frische HTML- und PDF-Ausgaben kontrollieren und vorhandene Programmieraufgaben samt Startcode-ZIP prüfen; Prüfumfang, Ergebnisse und Grenzen nachvollziehbar berichten.
---

# Lehrmaterial validieren

## Auftrag und Eingaben

Nutze diesen Skill nach Inhalts-, Layout- oder Buildänderungen sowie für die
abschließende Prüfung eines Materialsatzes. Ermittle betroffene Quellen,
Änderungsumfang und Ziel: gezielter Zwischenstand oder vollständiger Abschluss.
Eine Bitte um Validierung allein beauftragt keine Veröffentlichung.

Lies die [gemeinsame Arbeitsweise](../references/arbeitsweise.md), die
[Projektregeln](../../../AGENTS.md), relevante
[Fachregeln](../../../FACHREGELN.md) und je nach Inhalt die
[Vorlesungsregeln](../../../vorlesungen/AGENTS.md) bzw.
[Quizregeln](../../../quizacademy/AGENTS.md).
Bei Programmieraufgaben zusätzlich die [Code-Regeln](../../../code/labs/AGENTS.md)
und die [Paketkonventionen](../../../TEMPLATE.md#programmieraufgaben-mit-startcode) lesen.
Bewahre vorhandene Struktur und Inhalte. Melde notwendige größere
Umstellungen begründet; setze sie nur innerhalb des beauftragten Umfangs um.

## Umgebung aktivieren

Führe Befehle im Repository-Stamm aus. Prüfe die Installation anhand von
`scripts/toolchain.env` und `TEMPLATE.md`; nur falls nötig das Setup ausführen:

```bash
bash scripts/setup.sh
```

Aktiviere die Toolchain in jeder neuen Bash-Sitzung und führe den
gewählten Prüfbefehl in derselben Sitzung aus:

```bash
source scripts/activate.sh
python3 scripts/build-materials.py all
```

Browser, Node, Quarto oder TeX-Pakete nicht unabhängig vom Setup aktualisieren oder
installieren. System- und Netzwerkfehler diagnostizieren; Fehler nie als Erfolg melden.

## Prüfumfang wählen

| Ziel oder Änderung | Vorhandene Prüfung |
| --- | --- |
| Vortrag, Website oder deren Gestaltung | `python3 scripts/build-materials.py website` |
| Lernnotizen, Vortrag oder PDF-Gestaltung | zusätzlich `python3 scripts/build-materials.py handouts` |
| Labor, Probeklausur oder Lösungsprofil | `python3 scripts/build-materials.py labs` für beide Fassungen |
| Programmieraufgaben mit Manifest | `python3 scripts/build-materials.py labs`: zusätzlich Startcode, Referenzlösung und entpacktes Studierenden-ZIP |
| Buildskripte, Filter oder deren Konfiguration | passende Ausgabeformate und `python3 -m unittest discover -s tests -v` |
| Vollständiger Abschluss oder Veröffentlichungsbereitschaft | `python3 scripts/build-materials.py all` |

Wähle bei gemeinsamen Styles, Filtern oder Diagrammen alle betroffenen Formate.
Ein Website-Erfolg ersetzt keine PDF-Prüfung, eine gezielte Zwischenprüfung keinen
vollständigen Build vor dem Gesamtabschluss.
Quiz-CSV zusätzlich nach ihren Struktur- und Fachregeln prüfen; ein Quarto-Build
weist weder elf korrekte CSV-Spalten noch eine eindeutige richtige Antwort nach.
Verwende für angeforderte Quizdateien den [CSV-Validator](../quiz-erstellen/scripts/validate_quiz.py):

```bash
python3 .agents/skills/quiz-erstellen/scripts/validate_quiz.py quizacademy/datei.csv
```

Ersetze den Beispielpfad durch die tatsächlich geprüften Dateien. Ein erfolgreicher
Validatorlauf belegt das Format, nicht die fachliche Eindeutigkeit der Antworten.

## Frische Ausgaben kontrollieren

1. **Lauf belegen.** Prüfe Exitstatus und vollständige Meldungen. Der gemeinsame
   Builder rendert zunächst frisch unter `.quarto/materials-build/run-*` und
   übernimmt Ergebnisse erst nach Erfolg. Alte PDFs bei den Quellen gelten nicht
   als Nachweis. Fehlende oder leere Pflichtausgaben sind Fehler.
2. **Artefakte zuordnen.** Prüfe `_site/vorlesungen/<name>.html`,
   `_site/vorlesungen/<name>-handout.pdf` und gegebenenfalls
   `_site/labs/<name>.pdf` sowie `<name>-musterloesung.pdf`. Berücksichtige
   tatsächlich vorhandene Unterordner und die vom Builder gemeldeten Quellen.
3. **HTML ansehen.** Kontrolliere die betroffenen Folien und Diagramme bei
   vorgesehenem Reveal.js-Format, insbesondere neue oder geänderte Inhalte.
   Prüfe Überläufe, Formeln, Schrift, Kontrast, Links und Bildpfade.
   Lernnotizen gehören im Vortrag zur richtigen Folie in der Speaker View.
4. **PDF ansehen.** Prüfe Notizen auf Vollständigkeit und richtige Zuordnung,
   lesbare Abbildungen und Beschriftungen, Formeln, Seitenumbrüche und abgeschnittene
   Inhalte. Textauszug ist für Vollständigkeit hilfreich, ersetzt aber keine
   Sichtprüfung des Layouts. Bei umfangreichen Ausgaben den sichtgeprüften Umfang
   ausdrücklich nennen; Stichproben nicht als Prüfung jeder Seite ausgeben.
5. **Profile vergleichen.** Studierendenversion und Musterlösung separat öffnen.
   Aufgaben müssen in beiden vorhanden, Lösungen nur in der Lösungsfassung sichtbar
   sein. Prüfe insbesondere neue Aufgaben und geänderte Sichtbarkeitsblöcke.
6. **Fachliche Grenzen prüfen.** Rechenergebnisse, Modelle und Darstellungen mit
   Quellen und Fachregeln abgleichen; für ein ausführliches Review
   [material-review](../material-review/SKILL.md) verwenden. Ein erfolgreicher Build belegt keine fachliche
   oder didaktische Richtigkeit und keine Kalibrierung der Aufgabenschwierigkeit.
   Bei inhaltlichem Prüfumfang die Begriffswahl mit dem [Kursglossar](../../../GLOSSAR.md)
   und zusammengehörigen Materialien abgleichen; ein automatischer Build beweist keine semantische Terminologiekonsistenz.
7. **Programmierpakete prüfen.** Bei `code/labs/<lab_id>/lab.json` die Laufzeit,
   Befehle, Dateilisten und getrennten Erwartungen prüfen. `all` und `labs` müssen
   den Startervertrag, alle Referenzprüfungen und erneut den tatsächlich entpackten
   Download erfolgreich prüfen. Das ZIP unter `_site/labs/<lab_id>-startcode.zip`
   muss README, Startcode und freigegebene Tests vollständig enthalten; keine
   Referenzlösung oder Buildreste. Eine lokale erfolgreiche Ausführung aus dem
   Quellprojekt ersetzt den Test des entpackten Pakets nicht.

Erhalte beabsichtigte TODOs und fülle sie bei einer reinen Validierung nicht aus.
Vergleiche erwartete Starterfehlschläge exakt mit dem Manifest; ein allgemeiner
Rückgabecode ungleich null oder pauschales Ignorieren aller Testfehler reicht nicht.
Die Referenzlösung muss sämtliche deklarierten Prüfungen bestehen. Für zusätzliche
Lehrsprachen müssen passende Compiler/Laufzeiten tatsächlich eingerichtet sein.

Nutze verfügbare Browser-, PDF- und Bildwerkzeuge tatsächlich. Bei fehlenden
Werkzeugen die ausgelassene Kontrolle und deren Folge für den Abschluss nennen.
Livefigures nur als Syntaxreferenz verwenden; Mermaid und IEC-SVG folgen den Projektregeln.

## Ergebnis und Abschluss

Berichte Befehle, Quellen und frische Artefakte, bestandene und fehlgeschlagene
Prüfungen sowie ausgelassene Schritte mit Grund. Nenne tatsächliche Unit-Test-
Zahlen und übersprungene Tests; gerenderte Dateien sind keine Testfälle.
Behebe beauftragte Mängel und wiederhole die davon betroffenen Prüfungen.
Melde einen vollständigen Abschluss nur nach erfolgreichem Gesamtbuild und
den nötigen Inhaltskontrollen; sonst den erreichten Teilstand und offene Fehler.
Dieser Skill erstellt keinen automatischen Commit, Push, Pull Request oder Deploy.

Beispielaufrufe: „Validiere Vortrag und Handout.“ – „Prüfe vor dem Abschluss alle Materialien einschließlich beider Laborprofile.“
