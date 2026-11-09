# Abgleich mit dem DHBW-Quarto-Template

## Ausgangsstand und Umfang

Abgleich vom 9. November 2026 für `dantschi/echtzeitsysteme` mit
[`dantschi/dhbw-quarto-template`](https://github.com/dantschi/dhbw-quarto-template).

- Ursprünglicher Template-Stand: `efd484eabc72d9e357715b8c6d2c1b590aef89be` vom 24. Juli 2026.
- Erster Kurscommit: `9c0a1eb18a182e01fd449a1bc318d2e60b487b49`.
- Beide Commits besitzen denselben Git-Tree `a9db2c1ea636a01d1321eadb409125c92e5bc90e`.
- Geprüfter neuer Template-Stand: `578e3da2f55d82325007576e2a3043f4070dc85d` vom 7. November 2026.
- Umfang: sämtliche 13 Commits auf `main` nach dem Ausgangsstand. Generierte
  `gh-pages`-Commits enthalten keine zusätzlichen Autorenvorlagen und werden
  durch den Materialbuild neu erzeugt.

## Übernommene Änderungen

| Commit | Änderung | Umsetzung im Kurs |
|---|---|---|
| `9170284` | Website-Layout, Livefigures und EZS-Werkzeuge | Bereits enthalten; Renderer, Fonts und Styles mit dem Template verglichen. |
| `e589324` | Unteilbare Folienkarten im PDF | Bereits enthalten; spätere Kursverbesserungen an Notiznummerierung und Fortsetzungstiteln erhalten. |
| `a1654f3` | Vollständige Vorlesungsnotizen | Vorhandene Kursregel in die aktuellen Vorlesungsregeln überführt. |
| `9f73831` | QuizAcademy-Konventionen | Aktuelle Regeln und CSV-Validator übernommen; 25-Sekunden-Vorgabe des Kurses ergänzt. |
| `4398e10` | Migration der KI-Regeln und Fachregeln | Zentrale `AGENTS.md`, `FACHREGELN.md` und Bereichsregeln übernommen; alte Cursor-Regeln entfernt. |
| `413275d` | Merge der Regelmigration | Zusammen mit den übernommenen Regeländerungen berücksichtigt. |
| `ddc8350` | Gemeinsame Toolchain und vollständiger Build | Versionsgebundenes Setup, atomare Artefaktübernahme und getrennter CI-Build/Deploy übernommen. |
| `8eec6c3` | Sechs Autoren-Skills | Skills samt gemeinsamer Arbeitsweise übernommen. |
| `be1a8f6` | Geprüfte Startcode-Pakete | Paketierer, Manifestregeln und Tests übernommen; Suchbeispiel unter `examples/template/` getrennt. |
| `6a4bf0c` | Einheitliche Terminologie | Terminologieregeln übernommen; `GLOSSAR.md` an eingeführte EZS-Begriffe angepasst. |
| `ed1aad3` | Verständnis, Transfer und Prüfungsvorbereitung | Neue Aufgaben- und Reviewregeln in Projekt- und Skillanweisungen übernommen. |
| `144b88c` | Aktivierende Einstiegsfrage | Autorenregel übernommen; aktualisierte Beispielvorlesung unter `examples/template/vorlesungen/`. |
| `578e3da` | Template-Anleitung getrennt von Kursstartseite | `TEMPLATE.md` übernommen; `README.md` bleibt Kursstartseite mit Kursrahmen und Autorenverweisen. |

## Erhaltene Anpassungen

Die neun Vorlesungen, die bestehenden Übungsaufgaben einschließlich Probeklausur,
die Quizdateien und STM32-Codebausteine wurden nicht inhaltlich ersetzt. Erhalten
bleiben außerdem Kursnamen, Linkziele, lokale Folien-CSS-Dateien, Abbildungen,
Handout-Nummerierung und die Gestaltung der Übungs-PDFs.

Die neuen didaktischen Regeln gelten für künftige Bearbeitungen. Dieser
Infrastrukturabgleich ist keine rückwirkende Überarbeitung sämtlicher Einheiten.
Die Beispiel-Suchaufgabe und die generische Auftaktvorlesung gehören ausdrücklich
nicht zum EZS-Lehrplan; siehe [Beispielverzeichnis](../examples/template/README.md).

Der gemeinsame Builder schreibt frische Labor-PDFs nach `_site/labs/` und
`labs/_output/`. Bereits vorhandene, ignorierte `labs/*.pdf` werden nicht
überschrieben und gelten nicht als Nachweis eines aktuellen Builds. Die
Kurswebsite verwendet weiterhin die bisherigen öffentlichen PDF-Namen.

## Prüfbefehle

```bash
bash scripts/setup.sh
source scripts/activate.sh
python3 -m unittest discover -s tests -v
python3 scripts/build-materials.py all
python3 .agents/skills/quiz-erstellen/scripts/validate_quiz.py quizacademy/*.csv
```

Das getrennte Startcode-Beispiel wird zusätzlich gemäß
[Beispielanleitung](../examples/template/README.md#startcode-referenzlösung-und-zip-prüfen)
geprüft.

## Ergebnisse am 9. November 2026

- Alle 111 Netto-Dateiänderungen des Templates berücksichtigt: 94 Dateien sind
  byteidentisch übernommen bzw. bereits vorhanden, fünf alte Regeldateien wie
  vorgesehen gelöscht und zwölf Dateien kurs- oder pfadgerecht angepasst.
  Zusätzlich wurden drei lokal ergänzte Cursor-Regeln in die zentralen Regeln
  überführt. Alle 109 vorbestehenden Kursdateien unter `vorlesungen/`, `labs/`,
  `quizacademy/` und `code/` sind bytegenau erhalten. Die getrennt betrachtete
  Autorenvorlage `labs/lab_template.qmd` wurde an das aktuelle Template angepasst.
- `bash scripts/setup.sh` erfolgreich: Quarto 1.11.9, Node 24.22.1,
  TinyTeX 2026.11 mit TeX Live 2026, Chrome for Testing 157.0.8103.38 und
  rsvg-convert 2.60.0. Die im Template festgelegten Prüfsummen wurden geprüft.
- `python3 -m unittest discover -s tests -v`: 49 Tests erfolgreich, keine
  übersprungenen Tests.
- `python3 scripts/build-materials.py all`: Exitstatus 0. Frisch erzeugt wurden
  Startseite und neun Vorträge, neun PDF-Handouts sowie zehn Aufgabenblätter
  einschließlich Probeklausur in jeweils beiden Profilen, insgesamt 20 Labor-
  und Klausur-PDFs. Nach Abschluss der Dokumentationsdateien wurde `website`
  nochmals erfolgreich ausgeführt. Die anfänglich noch fehlenden Links auf
  Glossar und Abgleichbericht sind im finalen Websitebuild aufgelöst.
- Alle neun Quiz-CSV-Dateien formal validiert: acht mit zehn und die vom
  Kursautor bereitgestellte Fassung zu Vorlesung 5 mit zwölf Fragen. Inhalt und
  Fragenzahl wurden bei diesem Abgleich nicht verändert.
- Alle 447 Folien besitzen den zugehörigen Notizblock. Die PDF-Notiznummern
  decken jede Folie ab; Studentenversionen und Musterlösungen wurden für alle
  zehn Aufgabenblätter auf getrennte Sichtbarkeit geprüft.
- Automatisierte Browserprüfung mit der versionsgebundenen Chrome-Ausgabe:
  Startseite und alle neun Vorträge geöffnet, Notizzuordnung und geladene
  MathJax-Formeln geprüft. Erste Folie und jeweils erstes erkanntes Diagramm
  als Layoutstichprobe ohne Überläufe kontrolliert, insgesamt 17 Folien.
  MathJax 2.7.9 wurde für die Browserprüfung aus dem offiziellen npm-Paket lokal
  bereitgestellt; die veröffentlichten CDN-Verweise bleiben unverändert.
- Sichtgeprüft wurden die Startseite sowie die Diagrammfolien 12 aus Vorlesung 3
  und 9 aus Vorlesung 6. Zusätzlich PDF-Stichproben: Handout 1 Seite 3,
  Handout 5 Seite 19 und Übung 5, Studentenversion Seite 1 sowie Musterlösung
  Seite 6. Dies ist keine Sichtprüfung sämtlicher PDF-Seiten.
- Das getrennte Suchbeispiel wurde unter Python 3.12.14 im dokumentierten
  Starterzustand, als Referenzlösung und erneut aus dem entpackten ZIP geprüft.
  Das ZIP enthält genau die drei freigegebenen Dateien und keine Lösung;
  zusätzlich wurden 1.820 Eingabefälle der Referenz geprüft.
- Shellsyntax und `git diff --check` erfolgreich.

Es wurden keine STM32-Firmwareprojekte übersetzt oder auf Boards ausgeführt.
Der erfolgreiche Materialbuild ersetzt diese Nachweise nicht. Die bestehenden
STM32-Codebausteine werden durch die neue Infrastruktur nicht automatisch zu
vollständigen Firmwareprojekten. Ein GitHub-Actions-Lauf bzw. eine erneute
Veröffentlichung auf GitHub Pages wurde nicht ausgelöst.
