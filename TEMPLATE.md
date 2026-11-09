# DHBW Quarto Vorlesungstemplate — Einrichtung

> **Hinweis:** Dies ist ein **inoffizielles Dozenten-Template**, keine offizielle DHBW-Software.

GitHub-Template-Repository für Vorlesungswebsites an der **DHBW Stuttgart**: Quarto-Website, Reveal.js-Folien (Apple-/DHBW-Design), PDF-Handouts, Übungsblätter und Publish über GitHub Actions.

Dieses Repository ist bereits für **Echtzeitsysteme (EZS)** eingerichtet. Die [`README.md`](README.md) ist die Kursstartseite mit Kursrahmen, Lernzielen, Materialien und Literatur. Diese Datei (`TEMPLATE.md`) enthält die getrennte technische Anleitung für Einrichtung, Autorenwerkzeuge und Builds.

`index.qmd` übernimmt die README unverändert mit `{{< include README.md >}}` als **GitHub-Pages-Startseite**. Zusätzliche technische Erklärungen werden hier gepflegt. Der [Template-Abgleich](docs/template-sync.md) dokumentiert den übernommenen Stand und die erhaltenen Kursanpassungen.

## Was enthalten ist

| Bereich | Inhalt |
|---------|--------|
| Website | Quarto-Projekt (`_quarto.yml`); Startseite = `README.md` via `index.qmd` |
| Folien | Reveal.js unter `vorlesungen/*.qmd`, Design in `assets/styles/` |
| Diagramme | Mermaid, WaveDrom/PlantUML sowie Schaltpläne und berechnete Plots (siehe [Diagramm-Entscheidungsbaum](AGENTS.md#diagramm-entscheidungsbaum)) |
| Handout | Profil `handout` (`_quarto-handout.yml`) → PDF mit Folie + Dozentennotizen |
| Übungsblätter | `labs/*.qmd` mit Profil `solution`; Vorlage: `labs/lab_template.qmd` |
| Programmieraufgaben | Optionale Startcode-Projekte unter `code/labs/`; geprüfte ZIP-Downloads zum Aufgabenblatt |
| Build und CI/CD | Gemeinsame Skripte in `scripts/`; GitHub Actions prüft Website und PDFs, veröffentlicht auf `gh-pages` |
| KI-Kontext | Toolübergreifende Regeln in [`AGENTS.md`](AGENTS.md), ergänzt durch [`FACHREGELN.md`](FACHREGELN.md) sowie Regeln in `vorlesungen/` und `quizacademy/` |

## Neues Modul aus dem ursprünglichen Template anlegen

Die folgenden Einrichtungsschritte beziehen sich auf ein neues Repository aus dem [ursprünglichen DHBW-Quarto-Template](https://github.com/dantschi/dhbw-quarto-template). Die dortigen Beispieldateien und TODO-Markierungen sind in diesem bestehenden EZS-Kurs bereits durch Kursinhalte ersetzt. Für Änderungen am vorhandenen Kurs dessen Struktur und Inhalte erhalten.

1. Auf GitHub **Use this template** → neues Repository erstellen.
2. Repository klonen und die unten beschriebene [Build-Umgebung einrichten](#build-umgebung-einrichten).
3. Alle `[…]`-Platzhalter in `_quarto.yml`, im unteren Veranstaltungsteil der `README.md`, in `vorlesungen/01_kickoff.qmd` und optional in `AGENTS.md` ersetzen. Den vorbereiteten Kursrahmen samt Lernzielen ausfüllen und Kursübersicht, Literatur, Tools sowie Glossar anpassen.
4. Für die konkrete Kurswebsite den oberen Template-Block der README zwischen den TODO-Kommentaren vollständig einschließlich beider Markierungen löschen. Der Kursabschnitt ab `# [Modulname] ([Modulkürzel])` bleibt als Homepage erhalten; `index.qmd` behält seinen README-Include.
5. Weitere Vorlesungen als `vorlesungen/0N_….qmd` anlegen (`format: revealjs`, jede Folie mit `## Titel`, Notizen in `::: notes`). Nach der Titelfolie mit einer [aktivierenden Vorwissensfrage](vorlesungen/AGENTS.md#aktivierende-einstiegsfrage-je-vorlesung) beginnen; die Beispielfrage in `01_kickoff.qmd` passend zum neuen Thema ersetzen und später auflösen.
6. Übungsblätter als `labs/lab_NN_….qmd` anlegen (Vorlage: `labs/lab_template.qmd`).
7. Design/Livefigures möglichst unverändert lassen.

| Datei | Aktion |
|-------|--------|
| `_quarto.yml` | **Ersetzen:** `[Modulname]`, `[Modulkürzel]`, `[GITHUB-USER]`, `[REPO]` |
| `README.md` | **Anpassen:** unteren Kursabschnitt mit Kursrahmen, Lernzielen, Einheiten-Tabelle, Literatur und Tools ausfüllen; oberen Template-Block samt TODO-Markierungen löschen |
| `GLOSSAR.md` | **Anpassen:** Template-Beispiele durch die im Kurs eingeführten Begriffe, Synonyme und API-Zuordnungen ersetzen |
| `index.qmd` | **Beibehalten:** `{{< include README.md >}}` übernimmt die gesamte aktuelle README |
| `vorlesungen/01_kickoff.qmd` | **Ersetzen:** Folieninhalt und Notizen |
| `labs/lab_01_beispiel.qmd` | **Ersetzen** oder aus `lab_template.qmd` ableiten |
| `AGENTS.md`, `vorlesungen/AGENTS.md`, `quizacademy/AGENTS.md` | optional **Anpassen:** Modulbezug und Zielgruppe |

## Kurskontext festlegen

Vor dem Entwurf einer konkreten EIT-Vorlesung in der `README.md` den Abschnitt „Kursrahmen und Lernziele“ pflegen. Darin Modulname und -kürzel, Semester bzw. Studienrichtung, Vorkenntnisse, Präsenz- und Selbstlernzeit, Prüfungsform, Literatur und Werkzeuge zentral festhalten. Vorgaben aus einem vorliegenden Modulhandbuch mit dessen Fassung benennen; offene Angaben und Annahmen kenntlich machen.

Für informationstechnische Inhalte je nach Bedarf außerdem Programmiersprache und Standard bzw. Laufzeitversion, Zielplattform mit Controller/Kern und Architekturvariante, Toolchainversion mit relevanten Compileroptionen sowie das Zeitmodell dokumentieren. Die passenden Abschnitte der [Fachregeln](FACHREGELN.md) nach dem Inhalt der Einheit wählen; beispielsweise können bei einem Scheduling-Thema in Mikrocomputertechnik beide Fachabschnitte gelten.

Für jede Einheit wenige überprüfbare Lernziele formulieren und passende Übungen bzw. Verständnisprüfungen sowie ein Zeitbudget zuordnen. Die [Projektregeln](AGENTS.md#kurskontext-und-lernziele) beschreiben die Anforderungen an Lernziele, Quellen, Rechenprüfungen und reproduzierbare Laboraufgaben.

### Aufgaben für Verständnis und Prüfungsvorbereitung

Die [didaktischen Aufgabenregeln](AGENTS.md#verständnis-transfer-und-prüfungsvorbereitung) verlangen je Aufgabe ein Lernziel und einen eigenständigen Denkschritt sowie einen Abgleich mit den Folien. Halten Sie die Zuordnung und die Einschätzung von Anspruch, Hilfen und Bearbeitungszeit für Lehrende knapp in vorhandenen Notizen oder im Abschlussbericht fest; zusätzliche Dateien oder Quiz-CSV-Spalten sind dafür nicht nötig. Planen Sie eine zum Kurs passende Folge aus Einstieg, selbstständiger Anwendung bzw. Analyse und gegebenenfalls zusätzlicher Vertiefung. Die Erstellungs- und Review-Skills prüfen diese Kriterien ausdrücklich.

Dokumentierte Prüfungsvorgaben bestimmen die passenden Aufgabenarten und die erwartete Selbstständigkeit. Fehlende Vorgaben werden als offen oder als begründete Annahme ausgewiesen. Multiple-Choice-Fragen werden für offene Prüfungsaufgaben durch entsprechende Übungen ergänzt. Geschätzte Schwierigkeit und Zeiten sollten später anhand tatsächlicher Bearbeitungen und Rückmeldungen angepasst werden; ein erfolgreicher Materialbuild ist dafür kein Nachweis.

### Begriffe kursweit festlegen

Das in der README verlinkte [Kursglossar](GLOSSAR.md) ist die gemeinsame Quelle für bevorzugte Bezeichnungen, Bedeutung, Synonyme und die Zuordnung zu konkreten APIs. Die enthaltenen Beispiele beim Anlegen des Kurses ersetzen oder passend übernehmen; weitere wiederkehrende und leicht verwechselbare Begriffe bei ihrer Einführung ergänzen. Die [Terminologieregel](AGENTS.md#präzise-terminologie) gilt für sämtliche Materialien und wird von den Erstellungs- und Review-Skills berücksichtigt. Reale API-Namen bleiben erhalten; abweichende Bezeichnungen werden erklärt und fachlich zugeordnet.

## Mit KI-Tools arbeiten

Die Projektregeln stehen in normalen Markdown-Dateien im verbreiteten `AGENTS.md`-Format. KI-Tools mit entsprechender Unterstützung, einschließlich Cursor, können sie als Projektanweisungen einlesen:

| Datei | Geltungsbereich |
|-------|----------------|
| [`AGENTS.md`](AGENTS.md) | Kurskontext und Lernziele, akademische Genauigkeit, EIT-Rechen- und Laboraufgaben, Tech-Stack, Diagramme und lesbares Design |
| [`FACHREGELN.md`](FACHREGELN.md) | Je nach Inhalt zusätzliche Modellannahmen und Prüfungen für Informatik, Digitaltechnik, Mikrocomputertechnik und Realzeitsysteme |
| [`GLOSSAR.md`](GLOSSAR.md) | Kursbezogene Begriffsfestlegungen und Zuordnung von Synonymen und API-Namen |
| [`vorlesungen/AGENTS.md`](vorlesungen/AGENTS.md) | Zusätzlich für Vorlesungsfolien: vollständige Notizen und UTF-8-Prüfungen |
| [`quizacademy/AGENTS.md`](quizacademy/AGENTS.md) | Zusätzlich für Quiz-CSV-Dateien: Format, Lernzielbezug, Schwierigkeit und Antwortgestaltung |
| [`code/labs/AGENTS.md`](code/labs/AGENTS.md) | Zusätzlich für Programmieraufgaben: Startcode, Referenzlösung, Prüfvertrag und Studierendenpaket |

Die zentralen Regeln gelten immer zusätzlich zu den Regeln im jeweiligen Unterordner. `AGENTS.md` weist das KI-Tool an, für fachliche Inhalte die passenden Abschnitte aus `FACHREGELN.md` zu lesen. Bei einem KI-Tool ohne automatische `AGENTS.md`-Unterstützung die zentrale Datei und die für die Aufgabe relevanten ergänzenden Dateien ausdrücklich als Kontext übergeben. Die Regeln werden nur hier gepflegt; es sind keine separaten Cursor-Regeldateien erforderlich.

### Projekt-Skills verwenden

Unter `.agents/skills/` liegen sechs Aufgabenanleitungen im `SKILL.md`-Format. Die zentralen Regeln legen Qualitätsanforderungen fest; Skills beschreiben dazu Eingaben, Arbeitsschritte, Ergebnisse und Abschlussprüfungen.

| Skill | Beispielauftrag |
|-------|-----------------|
| [einheit-erstellen](.agents/skills/einheit-erstellen/SKILL.md) | „Erstelle eine Einheit zu endlichen Automaten mit vollständigen Lernnotizen.“ |
| [material-review](.agents/skills/material-review/SKILL.md) | „Prüfe diese Timer-Beispiele fachlich und didaktisch.“ |
| [uebungsblatt-erstellen](.agents/skills/uebungsblatt-erstellen/SKILL.md) | „Erstelle zur Einheit ein Übungsblatt samt Musterlösung.“ |
| [quiz-erstellen](.agents/skills/quiz-erstellen/SKILL.md) | „Erzeuge acht QuizAcademy-Fragen zur vorhandenen Vorlesung.“ |
| [kurs-planen](.agents/skills/kurs-planen/SKILL.md) | „Plane die Einheitenfolge aus diesen Modulvorgaben und dem Zeitbudget.“ |
| [material-validieren](.agents/skills/material-validieren/SKILL.md) | „Baue und prüfe alle Ausgabeformate des Kurses.“ |

Die automatische Erkennung von `.agents/skills/` hängt vom KI-Tool ab. Bei Bedarf die jeweilige `SKILL.md` ausdrücklich als Anleitung übergeben; sämtliche Verweise gelten relativ zu dieser Datei. Es ist keine automatische Aktivierung weiterer Skills erforderlich: Deren Prüfabläufe können direkt gelesen und die vorhandenen Skripte aufgerufen werden.

Bei Überarbeitungen erhalten die Skills standardmäßig die vorhandene Struktur und die Inhalte. Größere didaktische Umstrukturierungen werden begründet vorgeschlagen; bereits beauftragte Änderungen werden ohne zusätzliche Freigabeschritte bearbeitet. Die [gemeinsame Arbeitsweise](.agents/skills/references/arbeitsweise.md) beschreibt auch den Umgang mit fehlenden Kursangaben. Es müssen nicht für jeden Aufruf alle Kursdaten erneut eingegeben werden.

Der Quiz-Skill enthält einen Formatvalidator für eine oder mehrere CSV-Dateien:

```bash
python3 .agents/skills/quiz-erstellen/scripts/validate_quiz.py quizacademy/datei.csv
```

Dieser prüft das Dateiformat und die formalen Antwortbedingungen. Ob Fragen, Optionen und Erläuterungen fachlich richtig und didaktisch geeignet sind, wird gesondert geprüft. Die Skripttests laufen mit `python3 -m unittest discover -s tests -v`.

## Programmieraufgaben mit Startcode

Ein Übungsblatt kann optional ein Programmierprojekt enthalten. Aufgabenblatt und Code verwenden dieselbe Blattkennung; [Code-Regeln](code/labs/AGENTS.md) und der erweiterte Skill [uebungsblatt-erstellen](.agents/skills/uebungsblatt-erstellen/SKILL.md) beschreiben die Autorenanforderungen.

Unterstützt werden derzeit Projektordner direkt unter `code/labs/` mit der Kennung `lab_NN_thema` und dem zugehörigen Blatt direkt unter `labs/`. `NN` ist zweistellig; für das Thema sind ASCII-Buchstaben, Ziffern, Unterstriche und Bindestriche erlaubt. Kennungen mit `template` bleiben Vorlagen und sind nicht als Programmierprojekt zulässig.

```text
labs/lab_02_suchen.qmd
code/labs/lab_02_suchen/
  README.md
  lab.json
  starter/
  solution/
  tests/
```

Das [Python-Referenzbeispiel zur linearen Suche](examples/template/code/labs/lab_02_suchen/README.md) verwendet Python 3.12.x und ausschließlich die Standardbibliothek. Es demonstriert importierbaren Startcode mit nummerierten TODOs, eine Referenzlösung mit gleichen Schnittstellen und öffentliche Prüfungen. Es liegt getrennt unter [`examples/template/`](examples/template/README.md) und ergänzt keine weitere EZS-Unterrichtseinheit. Seine gesonderte Prüfung und Paketierung beschreibt die dortige README.

Die bestehenden STM32-Implementierungshilfen dieses Kurses liegen weiterhin unter `code/lab01` bis `code/lab09`, in den Aufgabenblättern und unter `labs/vertiefung/`. Neue Manifestprojekte können die unten beschriebene Konvention nutzen. Der gemeinsame Paketbuilder prüft nur vorhandene `code/labs/<lab_id>/lab.json`; er behauptet keinen Firmwarebuild der bisherigen STM32-Bausteine.

### Anleitung und Ausgangszustand

Die Projekt-README muss auch im entpackten Studierendenpaket verständlich sein: Sprache und Version, Einrichtung, konkrete Start-/Testbefehle, zu bearbeitende Dateien und erwartete Ergebnisse angeben. TODOs müssen eindeutig zu Teilaufgaben passen. Der Startcode ist standardmäßig baubar bzw. importierbar; fehlende Funktionen dürfen die ausdrücklich vorgesehenen Tests noch scheitern lassen. Falls Übersetzungsfehler selbst das Lernziel sind, müssen auch diese gezielt beschrieben und überprüft werden.

Die Referenzlösung erfüllt die Aufgabenstellung und besteht alle deklarierten Prüfungen. Sie ist mit den Erläuterungen im PDF synchron zu halten. Ein Validierungslauf ergänzt beabsichtigte TODOs nicht selbstständig.

### Manifest und Prüfvertrag

Jedes unterstützte Projekt besitzt eine [`lab.json`](examples/template/code/labs/lab_02_suchen/lab.json) mit Schema-Version `1`:

| Feld | Bedeutung |
|------|-----------|
| `version`, `language` | Manifestversion und Programmiersprache |
| `runtime.command`, `runtime.output_pattern` | Argumentliste der Versionsabfrage und regulärer Ausdruck für die gesamte, außen getrimmte Ausgabe |
| `student_files` | Ausdrückliche Liste der auszuliefernden Dateien relativ zum Projektordner, einschließlich README, Startcode und öffentlicher Tests |
| `solution_files` | Ergänzende Dateien unter `solution/` für die Referenzprüfung; sie kommen nicht in das ZIP |
| `checks` | Benannte Befehle als Argumentlisten mit getrennten Erwartungen für `starter` und `solution` |

Die Befehle laufen ohne Shell in isolierten Arbeitskopien mit `starter/` bzw. `solution/` als Arbeitsverzeichnis und einem Zeitlimit von 60 Sekunden pro Aufruf. Shell-Operatoren wie `&&` werden nicht ausgewertet; mehrere Prüfschritte als getrennte Einträge oder als eigenes Prüfskript deklarieren. Die Versionsabfrage prüft die tatsächliche Laufzeit; ein Quarto-Build ersetzt beispielsweise keinen C-Compiler oder eine Java-Laufzeit. Für eine neue Lehrsprache müssen die passende Version und ihre Einrichtung auch in der Cloud und in GitHub Actions festgelegt werden.

Dateilisten enthalten einzelne relative Dateipfade, keine Verzeichnisse oder Platzhaltermuster. Symlinks, versteckte Dateien und Verzeichnisse (etwa `.gitignore` oder `.vscode/`) sowie erkannte Cache- und Buildartefakte werden abgelehnt. `student_files` muss `README.md` und mindestens je eine Datei unter `starter/` und `tests/` enthalten; `solution_files` enthält ausschließlich Dateien unter `solution/`.

Jede Prüf-Erwartung enthält `exit_code`. Falls zusätzlich `json` angegeben ist, muss die Standardausgabe genau dieses JSON-Ergebnis liefern. Ein erwarteter Rückgabecode ungleich null benötigt stets ein solches Ergebnisprotokoll, etwa mit Testanzahl sowie den konkreten Fehlerkennungen. Dadurch wird ein Absturz oder zusätzlicher unerwarteter Fehler nicht pauschal als erwartetes Starterverhalten akzeptiert. Die Referenzlösung muss erfolgreiche Prüfungen deklarieren und bestehen.

### ZIP und gemeinsamer Build

```bash
source scripts/activate.sh
python3 scripts/build-materials.py labs
# oder vollständig wie in CI:
python3 scripts/build-materials.py all
# nur Codeprüfungen und ZIPs, ohne Quarto-Aufruf:
python3 scripts/build-materials.py code
```

`labs` und `all` prüfen vorhandene Programmierprojekte zusätzlich zu den PDF-Ausgaben; `code` prüft und paketiert nur die Projekte. Nur ausdrücklich in `student_files` genannte Dateien kommen in das Archiv; keine ungefilterten Verzeichnisse, Referenzlösungen, internen Tests, Caches oder Buildreste übernehmen. Das ZIP erhält den obersten Ordner `<lab_id>/`, wird frisch entpackt und erneut mit dem Startervertrag geprüft. Versionierte Quellen werden durch diese Prüfläufe nicht verändert. Fehlende Dateien, unpassende Laufzeiten und unerwartete Ergebnisse lassen den Build fehlschlagen.

Ein reguläres Kursprojekt wird nach erfolgreicher Prüfung unter `_site/labs/<lab_id>-startcode.zip` veröffentlicht. Auf der Website lautet der relative Link `labs/<lab_id>-startcode.zip`; aus dem direkt unter `labs/` liegenden Aufgabenblatt `<lab_id>-startcode.zip`. Referenzcode erhält kein automatisches Downloadpaket. Das separat aufbewahrte Suchbeispiel wird gemäß seiner [eigenen Anleitung](examples/template/README.md) geprüft und erzeugt kein zusätzliches Paket im normalen EZS-Kursbuild.

Ein öffentliches Git-Repository macht auch `solution/` lesbar. Getrennte ZIPs und das Quarto-Profil `solution` steuern die ausgelieferten Artefakte, verbergen aber keine Repository-Dateien. Sollen Lösungen erst später oder nur für Lehrende zugänglich sein, brauchen sie eine dafür geeignete getrennte Ablage. Die Beispielreferenz in diesem Template ist bewusst öffentlich.

## Build-Umgebung einrichten

Cloud-Umgebung und GitHub Actions verwenden dieselben Setup- und Build-Skripte. Das Setup unterstützt Linux x86_64 mit Debian 13 oder Ubuntu 24.04. Es benötigt `curl`, `python3`, `tar`, `xz`, `sha256sum` und `dpkg-deb`; die Systemabhängigkeiten für den Browser und die SVG-Konvertierung installiert der erste Aufruf mit Root-Rechten bzw. `sudo`:

```bash
bash scripts/setup.sh --system-deps
bash scripts/setup.sh
source scripts/activate.sh
```

Das Setup prüft die Downloads mit SHA-256 und installiert folgende festgelegte Versionen im nicht versionierten Verzeichnis `.tools/` des Repositorys. Versionen und Prüfsummen werden zentral in [`scripts/toolchain.env`](scripts/toolchain.env) gepflegt:

| Werkzeug | Version |
|----------|---------|
| Quarto | 1.11.9 |
| Node.js | 24.22.1 |
| Chrome Headless Shell für Mermaid-PDFs | 157.0.8103.38 |
| TinyTeX mit vollständigem Paketbestand | 2026.11 |
| `rsvg-convert` für SVGs in PDFs | 2.60.0 |

`source scripts/activate.sh` in jeder neuen Shell ausführen. Die Aktivierung wählt die installierten Werkzeuge einschließlich Browser und TeX und berücksichtigt die Proxy-Konfiguration der Cloud-Umgebung. Das Setup kann erneut ausgeführt werden. Für eine gemeinsame Installation außerhalb des Checkouts kann vor Setup und Aktivierung `DHBW_TOOLCHAIN_DIR` auf ein beschreibbares Verzeichnis gesetzt werden. Im Build werden fehlende TeX-Pakete nicht automatisch nachinstalliert; nötige Änderungen am Paketbestand gehören zur gemeinsamen Toolchain.

Die festgelegten Renderwerkzeuge verringern Unterschiede zwischen Cloud und CI. Betriebssystem und Systembibliotheken bleiben Teil der Umgebung; bytegleiche Ausgaben über verschiedene Betriebssysteme hinweg sind damit nicht garantiert. PlantUML benötigt bei einem Cache-Miss weiterhin Zugriff auf den konfigurierten Kroki-Dienst.

## Lokal arbeiten

```bash
# Vollständige Prüfung wie in GitHub Actions
python3 scripts/build-materials.py all

# Bei der Bearbeitung gezielt einen Bereich bauen
python3 scripts/build-materials.py website
python3 scripts/build-materials.py handouts
python3 scripts/build-materials.py labs
# Kompatibler Kurzaufruf für beide Lab-Versionen
bash build_labs.sh

# Vorschau nach Aktivierung der Build-Umgebung
quarto preview
quarto preview vorlesungen/01_was-ist-echtzeit.qmd
```

Der gemeinsame Build erstellt die Website, zu allen Vorlesungen unter `vorlesungen/` ein PDF-Handout und zu nummerierten Übungsblättern sowie `probeklausur.qmd` beide PDF-Versionen. Bei Programmieraufgaben prüft er zusätzlich die deklarierten Codevarianten und entpackten Startcode-Pakete. Er berücksichtigt bei den Quarto-Quellen auch Unterordner; Vorlagen sowie versteckte und interne Verzeichnisse bleiben ausgenommen. Er rendert in ein frisches Arbeitsverzeichnis und übernimmt die Ergebnisse erst, wenn alle angeforderten Renderläufe und Codeprüfungen erfolgreich waren. Fehlende, leere oder fehlgeschlagene Pflichtausgaben führen zu einem Fehler.

Die veröffentlichbaren Dateien liegen gemeinsam in `_site/`:

| Ausgabe für die Beispieldateien | Pfad |
|--------------------------------|------|
| Folien | `_site/vorlesungen/01_was-ist-echtzeit.html` |
| PDF-Handout | `_site/vorlesungen/01_was-ist-echtzeit-handout.pdf` |
| Übungsblatt für Studierende | `_site/labs/lab_01_bare-metal-polling-interrupts.pdf` |
| Musterlösung | `_site/labs/lab_01_bare-metal-polling-interrupts-musterloesung.pdf` |
| Startcode-Paket für ein vorhandenes Manifestprojekt | `_site/labs/<lab_id>-startcode.zip` |

Für bestehende lokale Abläufe werden Handouts zusätzlich unter `_handout/vorlesungen/` und Lab-PDFs mit den Suffixen `-studierende.pdf` bzw. `-musterloesung.pdf` unter `labs/_output/` abgelegt. Der gemeinsame Builder aktualisiert keine bisherigen `labs/<name>.pdf` im Quellordner; die frischen Studierendenfassungen liegen unter `_site/labs/<name>.pdf` und `labs/_output/<name>-studierende.pdf`. Vorhandene PDFs im Quellordner sind kein Nachweis für einen erfolgreichen Build. Nach einem fehlgeschlagenen Build können dort noch Ergebnisse eines früheren erfolgreichen Laufs liegen; veröffentlicht werden darf nur nach erfolgreichem vollständigem Build.

Das reguläre Handout verwendet das Profil `handout` und PDF als Zielformat. Optional erzeugt `quarto render vorlesungen/01_was-ist-echtzeit.qmd --profile handout --to revealjs` ein HTML-Handout mit sichtbaren Notizen; es gehört nicht zum vollständigen Pflicht-Build. Vortrag und Handouts haben eigene Layoutvorgaben; das Erweiterungsformat `handout-revealjs` ist eine zusätzliche Variante. Details und Prüfungen stehen unter [Ausgabeformate und Prüfung](AGENTS.md#ausgabeformate-und-prüfung), die Werkzeugwahl unter [Diagramm-Entscheidungsbaum](AGENTS.md#diagramm-entscheidungsbaum).

## Publish mit GitHub Actions

Workflow: [`.github/workflows/publish.yml`](.github/workflows/publish.yml)

**Trigger:** Pull Request, Push auf `main` oder manuell (*Actions* → *Build and publish Quarto materials*).

Der Build-Job prüft die Build-Skripte, richtet die gemeinsame Toolchain ein und führt `python3 scripts/build-materials.py all` aus. Website, PDF-Handouts und beide Versionen der Übungsblätter müssen erfolgreich entstehen; vorhandene Programmierprojekte müssen ihren getrennten Starter-/Referenzvertrag und den Test des entpackten ZIPs erfüllen. Die vollständige Website einschließlich PDFs und Startcode-Paketen steht bei einem erfolgreichen Lauf 14 Tage als Artefakt `course-materials` zum Herunterladen bereit; Pull Requests veröffentlichen nichts.

Ein eigener Deploy-Job veröffentlicht nur auf `main` und nur nach erfolgreichem Build die bereits geprüften Dateien auf **`gh-pages`**. Er rendert nicht erneut und installiert über `scripts/setup.sh --quarto-only` nur das dafür benötigte Quarto. Nur dieser Job erhält Schreibrechte auf Repository-Inhalte.

**Einmalig:** Branch `gh-pages` anlegen und unter *Settings → Pages* als Veröffentlichungsquelle auswählen (*Deploy from a branch*). Die Repository-Richtlinien müssen dem Deploy-Job die angeforderten Schreibrechte erlauben. Für öffentliche Kursseiten das Repository **public** stellen.

**Hinweis:** Hochschullogos und Marken sind nicht Teil der CC-BY-Lizenz — siehe [`LICENSE`](LICENSE) und den Abschnitt *OER und Lizenz* in der `README.md`.
