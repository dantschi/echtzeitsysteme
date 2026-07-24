<!-- ==========================================================================
     TODO — LÖSCHEN (nach dem Kopieren des Templates)
     Gesamten Block von TEMPLATE-START bis TEMPLATE-ENDE entfernen,
     damit er nicht auf der GitHub-Pages-Startseite erscheint.
     ========================================================================== -->
<!-- TEMPLATE-START -->

# DHBW Quarto Vorlesungstemplate

> **Hinweis:** Dies ist ein **inoffizielles Dozenten-Template**, keine offizielle DHBW-Software.

> **Hinweis zur Startseite:** Diese `README.md` wird von `index.qmd` eingebunden und ist damit der Inhalt der **GitHub-Pages-Startseite** (und der Repo-README auf GitHub). Nach dem Einrichten Ihres Moduls den gesamten Template-Block (**TODO — LÖSCHEN**) entfernen und unten nur noch den Kursinhalt belassen (**TODO — ERSETZEN**).

GitHub-Template-Repository für Vorlesungswebsites an der **DHBW Stuttgart**: Quarto-Website, Reveal.js-Folien (Apple-/DHBW-Design) und automatisches Publish inkl. PDF-Handout über GitHub Actions.

## Was enthalten ist

| Bereich | Inhalt |
|---------|--------|
| Website | Quarto-Projekt (`_quarto.yml`); Startseite = diese README via `index.qmd` |
| Folien | Reveal.js unter `vorlesungen/*.qmd`, Design in `assets/styles/` |
| Handout | Profil `handout` (`_quarto-handout.yml`) → PDF mit Folie + Dozentennotizen |
| Übungsblätter | `labs/*.qmd` mit Profil `solution` (Studierende / Musterlösung) |
| CI/CD | `.github/workflows/publish.yml` → Website auf `gh-pages` + Handout- + Lab-PDFs |
| KI-Kontext | Cursor-Rules unter `.cursor/rules/` |

## Neues Modul anlegen

1. Auf GitHub **Use this template** → neues Repository erstellen.
2. Repository klonen; mit [Quarto](https://quarto.org/) lokal arbeiten.
3. **TODO — ERSETZEN:** Alle `[…]`-Platzhalter in `_quarto.yml`, in diesem Dateiteil unten (Kursinhalt), in `vorlesungen/01_kickoff.qmd` und optional in `.cursor/rules/`.
4. **TODO — LÖSCHEN:** Diesen gesamten Template-Abschnitt (bis `TEMPLATE-ENDE`) entfernen.
5. Weitere Vorlesungen als `vorlesungen/0N_….qmd` anlegen (`format: revealjs`, Notizen in `::: notes`).
6. Übungsblätter als `labs/lab_NN_….qmd` anlegen (Lösungen in `::: {.content-visible when-profile="solution"}`).
7. Design/Mermaid in `assets/styles/` möglichst unverändert lassen.

| Datei | Aktion |
|-------|--------|
| `_quarto.yml` | **Ersetzen:** `[Modulname]`, `[Modulkürzel]`, `[GITHUB-USER]`, `[REPO]` |
| `README.md` (dieser Block) | **Löschen** nach dem Einrichten |
| `README.md` (Kursinhalt unten) | **Ersetzen:** Beschreibung, Literatur, Tools |
| `index.qmd` | Titel anpassen; behält `{{< include README.md >}}` |
| `vorlesungen/01_kickoff.qmd` | **Ersetzen:** Folieninhalt und Notizen |
| `labs/lab_01_beispiel.qmd` | **Ersetzen** oder weitere `lab_NN_….qmd` ergänzen |
| `.cursor/rules/dhbw-*.mdc` | optional **Ersetzen:** Modulbezug |

## Lokal arbeiten

```bash
quarto preview
quarto preview vorlesungen/01_kickoff.qmd
quarto render vorlesungen/01_kickoff.qmd --profile handout --to pdf
# → _handout/vorlesungen/01_kickoff.pdf

# Übungsblätter: Studierende bzw. Musterlösung
quarto render labs/lab_01_beispiel.qmd --to pdf
quarto render labs/lab_01_beispiel.qmd --to pdf --profile solution
# oder beide Varianten gebündelt:
./build_labs.sh
# → labs/_output/*-studierende.pdf und *-musterloesung.pdf
```

PDF-Handout: Quarto, TinyTeX/LaTeX; für Mermaid: `quarto install chrome-headless-shell`.

**Übungsblätter:** Lösungen stehen in Divs mit `when-profile="solution"` und erscheinen nur mit `--profile solution` (siehe `labs/_quarto-solution.yml`). Dateien mit `template` im Namen werden in der CI übersprungen.
## Publish mit GitHub Actions

Workflow: [`.github/workflows/publish.yml`](.github/workflows/publish.yml)

**Trigger:** Push auf `main` oder manuell (*Actions* → *Publish Quarto Website*).

**Ablauf:** Website rendern → PDF-Handouts für alle `vorlesungen/*.qmd` → Übungsblatt-PDFs (Studierende + Musterlösung) nach `_site/labs/` → Publish auf Branch **`gh-pages`**.

Scheitert ein Handout oder Übungsblatt (z. B. Mermaid/Chrome), erscheint eine **Warnung**; die Website wird trotzdem veröffentlicht und der Job bleibt grün.

Nach dem Publish sind Materialien z. B. erreichbar unter:

- Folien: [`vorlesungen/01_kickoff.html`](vorlesungen/01_kickoff.html)
- PDF-Handout: [`vorlesungen/01_kickoff-handout.pdf`](vorlesungen/01_kickoff-handout.pdf)
- Übungsblatt: [`labs/lab_01_beispiel.pdf`](labs/lab_01_beispiel.pdf)
- Musterlösung: [`labs/lab_01_beispiel-musterloesung.pdf`](labs/lab_01_beispiel-musterloesung.pdf)
Vollständige URL typischerweise `https://[GITHUB-USER].github.io/[REPO]/` (Startseite = Inhalt dieser README über `index.qmd`).

**Einmalig:** Branch `gh-pages` vorhanden; *Settings → Pages* → Deploy from branch `gh-pages`; Actions mit Schreibrechten; für öffentliche Kursseiten Repo **public** stellen.

**Hinweis:** Dies ist ein **inoffizielles Dozenten-Template**, keine offizielle DHBW-Software. Das DHBW-Logo (`assets/dhbw_logo.svg`) und andere geschützte Markenzeichen sind nicht Teil der CC-BY-Lizenz — siehe [`LICENSE`](LICENSE) und den Abschnitt *OER & Lizenz* unten.

<!-- TEMPLATE-ENDE -->

<!-- ==========================================================================
     TODO — ERSETZEN (Kursinhalt für Studierende / GitHub Pages)
     Platzhalter […] durch Ihre Moduldaten ersetzen.
     ========================================================================== -->

# [Modulname] ([Modulkürzel])

Herzlich willkommen zur Vorlesung **[Modulname] ([Modulkürzel])** an der Dualen Hochschule Baden-Württemberg Stuttgart.

[Beschreibung]

## Materialien

| Format | Link |
|--------|------|
| Folien (Reveal.js) | [Kickoff](vorlesungen/01_kickoff.html) |
| PDF-Handout (Skript) | [Kickoff-Handout (PDF)](vorlesungen/01_kickoff-handout.pdf) |
| Übungsblatt | [Beispielblatt (PDF)](labs/lab_01_beispiel.pdf) |
| Musterlösung | [Beispielblatt Musterlösung (PDF)](labs/lab_01_beispiel-musterloesung.pdf) |

Das PDF-Handout enthält die Folien inklusive Dozentennotizen. Übungsblätter und Musterlösungen werden bei jedem Publish über GitHub Actions aktualisiert.
## Literatur

Die zentrale Referenz dieses Moduls ist:

> **[Autor:innen]**  
> *[Werktitel]*  
> [Verlag]

Definitionen, Terminologie und didaktischer Aufbau orientieren sich an diesem Standardwerk.

## Tools & Ressourcen

| Tool | Einsatz | Link |
|------|---------|------|
| **[Tool 1]** | [Beschreibung/Tools] | [Link] |
| **[Tool 2]** | [Beschreibung/Tools] | [Link] |

## OER & Lizenz

Dieses Vorlesungsmaterial ist eine **Open Educational Resource (OER)**. Die Vorlesungsinhalte – Texte, Code und Diagramme – stehen unter der Lizenz **[Creative Commons Attribution 4.0 International (CC BY 4.0)](https://creativecommons.org/licenses/by/4.0/)**.

Sie dürfen diese Inhalte teilen und bearbeiten, sofern Sie die Urheberschaft angemessen nennen. Der vollständige Lizenztext liegt in der Datei [`LICENSE`](LICENSE) im Repository.

**Ausnahme:** Hochschullogos und andere geschützte Markenzeichen (einschließlich des DHBW-Logos) sind von dieser Lizenz **ausdrücklich ausgenommen**. Sie unterliegen dem Markenrecht der jeweiligen Rechteinhaber und dürfen ohne deren Genehmigung nicht übernommen oder weiterverwendet werden.
