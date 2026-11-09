# Anweisungen für KI-Tools

Diese Datei enthält die gemeinsamen Projektregeln für KI-Tools und gilt für das gesamte Repository. Fach- und Dateikonventionen gelten jeweils für die unten genannten Inhalte bzw. Dateitypen.

Lies vor dem Erstellen oder Bearbeiten von Vorlesungsfolien zusätzlich [vorlesungen/AGENTS.md](vorlesungen/AGENTS.md), vor der Arbeit an Quiz-CSV-Dateien zusätzlich [quizacademy/AGENTS.md](quizacademy/AGENTS.md). Diese Dateien ergänzen die zentralen Regeln für ihren jeweiligen Bereich.

Für Programmieraufgaben mit Startcode gelten zusätzlich die [Regeln für Codeprojekte](code/labs/AGENTS.md).

Lies für Inhalte zu Informatik, Digitaltechnik, Mikrocomputertechnik und Realzeitsystemen außerdem die jeweils einschlägigen Abschnitte in [FACHREGELN.md](FACHREGELN.md). Entscheidend ist der Inhalt einer Aufgabe oder eines Beispiels, nicht allein der Modulname; mehrere Fachabschnitte können gemeinsam gelten.

## Aufgabenbezogene Skills

Für wiederkehrende Arbeitsaufträge stehen projektbezogene Skills unter `.agents/skills/` bereit. Verwende bei passender Aufgabe die jeweilige Anleitung; diese Datei und die fachlichen Regeln gelten weiterhin. Die Skills teilen die [gemeinsame Arbeitsweise](.agents/skills/references/arbeitsweise.md), einschließlich des Erhalts vorhandener Struktur und Inhalte bei Überarbeitungen.

| Aufgabe | Skill |
|---------|-------|
| Vorlesungseinheit erstellen oder überarbeiten | [einheit-erstellen](.agents/skills/einheit-erstellen/SKILL.md) |
| Fachliche und didaktische Prüfung | [material-review](.agents/skills/material-review/SKILL.md) |
| Übungsblatt mit Musterlösung erstellen oder überarbeiten | [uebungsblatt-erstellen](.agents/skills/uebungsblatt-erstellen/SKILL.md) |
| QuizAcademy-Fragen erzeugen oder überarbeiten | [quiz-erstellen](.agents/skills/quiz-erstellen/SKILL.md) |
| Kursrahmen, Lernziele und Einheitenfolge planen | [kurs-planen](.agents/skills/kurs-planen/SKILL.md) |
| Gerenderte Materialien und Build-Ergebnisse prüfen | [material-validieren](.agents/skills/material-validieren/SKILL.md) |

## Projektkontext

Unterstütze den Entwurf von Vorlesungsmaterial für das in `README.md` beschriebene Modul an der DHBW Stuttgart mit fachlicher, didaktischer und technischer Expertise.

- **Zielgruppe:** Bachelorstudierende der Elektro- und Informationstechnik an der DHBW Stuttgart; Semester, Studienrichtung und Vorkenntnisse gemäß Kurskontext
- **Sprache:** Deutsch
- **Anrede:** Professionelles „Sie“
- **Tonfall:** Professionell, akademisch präzise, didaktisch greifbar und motivierend

### Arbeitsweise

- Code, SCSS und Folien müssen für ihren angegebenen Zweck korrekt und vollständig sein. Didaktisch reduzierter Lehrcode und Pseudocode sind erlaubt; kennzeichne Vereinfachungen, ausgelassene Fehlerbehandlung und Unterschiede zur produktiven Implementierung. Keine unnötigen Platzhalter außer den vorgesehenen Template-Platzhaltern wie `[Modulname]`.
- Komplexe Code-Anpassungen im Chat kurz und prägnant erklären
- Modulspezifische Bezüge (Literatur, Tools, Fachbegriffe) an den Einträgen in `README.md` und `_quarto.yml` orientieren

### Kurskontext und Lernziele

- Dokumentiere den Kurskontext zentral in `README.md` im Abschnitt „Kursrahmen und Lernziele“: Modulname und -kürzel, Semester bzw. Studienrichtung, vorausgesetzte Kenntnisse, verfügbare Präsenz- und Selbstlernzeit, vorgesehene Prüfungsform sowie Literatur und Werkzeuge. Halte Titel und Modulbezüge in `_quarto.yml` dazu konsistent.
- Ergänze je nach Inhalt Programmiersprache und Standard bzw. Laufzeitversion, Zielplattform mit relevantem Controller/Kern und Architekturvariante, Toolchainversion und relevante Compileroptionen sowie das verwendete Zeitmodell. Benenne die Fachschwerpunkte, deren Regeln gelten. Setze eine bestimmte Wortbreite, Hardware oder Toolchain nicht stillschweigend voraus.
- Übernimm Vorgaben aus dem tatsächlich vorliegenden Modulhandbuch und benenne dessen Fassung. Erfinde keine offiziellen Lernziele, Prüfungsanforderungen oder Zeitvorgaben. Kläre fehlende Angaben, die den Entwurf wesentlich bestimmen; kennzeichne sonst begründete Annahmen im Entwurf.
- Formuliere pro Einheit wenige beobachtbare Lernziele, etwa „berechnen“, „dimensionieren“, „messen“, „erklären“ oder „beurteilen“. Ordne jedem Lernziel eine passende Übung oder Verständnisprüfung zu; Aufgaben und Prüfungsniveau müssen zu den Lernzielen passen.
- Plane ein realistisches Zeitbudget einschließlich Übungen, Fragen und Besprechung. Nutze, wo fachlich passend, nachvollziehbare Anwendungsfälle aus der beruflichen Praxis, um Theorie und Anwendung zu verbinden.

## Inhaltliche & akademische Genauigkeit

### Präzise Terminologie

- Verwende für dasselbe Konzept die eingeführte bevorzugte Bezeichnung durchgängig in Folien, Lernnotizen, Abbildungen, Übungen, Musterlösungen, Quizfragen und Codebeispielen. Trenne fachlich unterschiedliche, leicht verwechselbare Konzepte; eine einheitliche Wortwahl darf Bedeutungsunterschiede nicht verdecken.
- Pflege wiederkehrende oder leicht verwechselbare Kursbegriffe zentral in [GLOSSAR.md](GLOSSAR.md): bevorzugte Bezeichnung, Bedeutung und Geltungsbereich, zulässige Synonyme bzw. englische Bezeichnungen, gegebenenfalls API-Zuordnung sowie die Einführung im Kurs. Lies das Glossar vor dem Erstellen oder Überarbeiten von Materialien. Die enthaltenen Template-Beispiele sind für den konkreten Kurs anzupassen, keine Vorgabe für dessen Themenwahl.
- Ordne Synonyme, Abkürzungen und abweichende englische Bezeichnungen bei ihrer Einführung ausdrücklich dem Kursbegriff zu. Wechsle nicht ohne Erklärung zwischen ihnen. Führe in Übungen und Quiz keine unerklärten Alternativbezeichnungen ein, wenn das Lernziel nicht gerade deren Zuordnung ist. Selbstständig nutzbare Materialien benötigen die zum Verständnis nötige lokale Erklärung oder einen präzisen Verweis.
- Erhalte tatsächliche API-Namen und vorgegebene Codebezeichner. Erkläre ihre Zuordnung zum fachlichen Begriff und relevante semantische Unterschiede. Verwende in generischem Pseudocode konsistente Operationsnamen. Beispielsweise sind ein Mutex als Objekt und das Erwerben bzw. Freigeben dieses Objekts zu unterscheiden; `acquire` und `lock` sind nicht kontextfrei austauschbar. Keine pauschalen Textersetzungen in Code, Zitaten oder API-Namen.
- Gleiche bei Änderungen die Terminologie mit den zugehörigen Materialien und dem Glossar ab. Ergänze fehlende Festlegungen anhand der vorhandenen Einführung und Literatur. Benenne widersprüchliche Verwendungen mit Fundstellen; erfinde keine neue Benennung, um den Konflikt zu umgehen. Bei beauftragten Begriffskorrekturen Glossar und betroffene Materialien gemeinsam konsistent halten.

### Literaturbezug

Erklärungen und Definitionen an der zentralen Modul-Literatur ausrichten (siehe Platzhalter in `README.md`: `[Autor:innen]`, `[Werktitel]`). Belege übernommene Abbildungen, spezifische technische Kennwerte und normative Aussagen mit einer nachvollziehbaren Quelle. Verwende für Bauteil- und Schnittstellenangaben möglichst die zugehörigen Datenblätter, Normen oder Herstellerdokumentationen.

Nenne bei Literatur die Ausgabe und eine geprüfte Fundstelle, bei Datenblättern und Normen die Dokumentkennung bzw. Version und den relevanten Abschnitt. Erfinde keine Quellen, Seitenzahlen oder Norminhalte. Ist eine Aussage nicht anhand der verfügbaren Quellen überprüfbar, benenne diese Grenze und verwende sie nicht als gesicherte technische Vorgabe.

### Didaktischer Ansatz

Konzepte klar und schrittweise aufbauen — von Grundlagen zu Anwendungen. Den in den Folien etablierten roten Faden (`[Ebene 1] → … → [Ebene n]`) konsistent fortführen.

Beginne jede Vorlesungseinheit mit einer Frage, die das Vorwissen aktiviert und vorhandene Vorstellungen zum Thema herausfordert. Platzierung, Durchführung, vollständige Notizen und spätere Auflösung stehen in den [Folienregeln zur Einstiegsfrage](vorlesungen/AGENTS.md#aktivierende-einstiegsfrage-je-vorlesung).

### Verständnis, Transfer und Prüfungsvorbereitung

- Bestimme für jede Übungsaufgabe und Quizfrage das Lernziel und den eigenständigen Denkschritt: beispielsweise einen Ablauf herleiten, einen Fehler diagnostizieren, eine Lösung begründen oder die Folgen geänderter Randbedingungen beurteilen. Halte diese Zuordnung für Lehrende knapp in vorhandenen Autorennotizen oder im Abschlussbericht fest. Begriffs- und Routineaufgaben sind als Einstieg und für entsprechende Lernziele sinnvoll.
- Prüfe die Foliennähe anhand des Lösungswegs: Genügen Wiederfinden, sprachliches Zuordnen oder das Übernehmen eines bereits vollständig gelösten Beispiels? Auch eine Paraphrase oder ein reiner Zahlenwechsel kann eine Routineaufgabe bleiben. Verwende für Anwendungs- und Transferziele neue, mit eingeführten Konzepten lösbare Situationen oder fachliche Entscheidungen. Behalte die eingeführte Terminologie bei und stelle alle erforderlichen Angaben bereit; unbekanntes Zusatzwissen darf die Lösung nicht bestimmen.
- Stimme den gesamten Aufgabensatz auf die Lernziele ab. Bei Anwendungs- und Analysezielen wenige Einstiege, einen Schwerpunkt auf selbstständiger Anwendung und Analyse sowie gegebenenfalls ausdrücklich gekennzeichnete zusätzliche Vertiefung vorsehen. Vermeide starre Quoten. Stimme Hilfen und Teilaufgaben auf den Lernstand ab; prüfungsnahe Aufgaben sollen die dort erwartete Selbstständigkeit einfordern.
- Schätze den Anspruch anhand der Vorkenntnisse, erforderlichen Denk- und Rechenschritte, Hilfen, Hilfsmittel und Bearbeitungszeit ein. Ein anspruchsvolles Lernziel und eine schwierige Aufgabe sind nicht automatisch gleichbedeutend. Erzeuge Schwierigkeit nicht allein durch Fangfragen, sprachliche Hürden oder unnötige Rechenlast. Begründe die Einschätzung knapp für Lehrende und kennzeichne geschätzte Zeiten; nutze vorhandene Bearbeitungszeiten, typische Fehler und Rückmeldungen von Studierenden zur späteren Kalibrierung.
- Gleiche Aufgabenarten, Lernzielabdeckung, Umfang und Selbstständigkeit mit den tatsächlich dokumentierten Prüfungsanforderungen und Hilfsmitteln ab. Zur Vorbereitung auf offene Rechen-, Begründungs- oder Programmieraufgaben gehören entsprechende Übungen zusätzlich zu Multiple-Choice-Fragen. Bei fehlenden Prüfungsvorgaben begründete Annahmen kenntlich machen und keine gesicherte Übereinstimmung mit einer Prüfung behaupten.
- Prüfe vor dem Abschluss Unter- und Überforderung sowie Foliennähe anhand konkreter Aufgaben und ihrer Lösungen. Benenne den zu einfachen oder überfordernden Lösungsweg und schlage eine zum Lernziel passende Anpassung vor. Ein erfolgreicher Build oder CSV-Formatcheck belegt weder Verständnisförderung noch einen empirisch angemessenen Schwierigkeitsgrad.

### Tool-Integration

Wo möglich konkrete Bezüge zu den Kurs-Tools herstellen (siehe `README.md`: `[Tool 1]`, `[Tool 2]`). Diagramm-Werkzeuge auf Folien nach [Diagramm-Entscheidungsbaum](#diagramm-entscheidungsbaum) wählen.

### Formeln & Notation

Zwingend LaTeX (`$...$` oder `$$...$$`) für Formeln, Gleichungen und formale Notation.

### Ingenieurwissenschaftliche Rechenprüfung

Wende Einheiten- und Dimensionsprüfungen bei physikalischen Größen an. Für logische und softwarebezogene Beispiele sind insbesondere die unten beschriebenen [Modellannahmen und fachlichen Prüfungen](#modellannahmen-und-fachliche-prüfungen) maßgeblich.

- Definiere Größen, Symbole und Einheiten konsistent; verwende SI-Einheiten bzw. benenne verwendete Abweichungen und Umrechnungen. Trenne Zahlenwert und Einheit typografisch, etwa `$U = 5\,\mathrm{V}$`.
- Prüfe Gleichungen auf Dimensionskonsistenz und Ergebnisse auf Größenordnung, sinnvolle Grenzfälle und physikalische Plausibilität. Prüfe Rechenbeispiele nach Möglichkeit mit einer unabhängigen Rechnung oder einem nachvollziehbaren Skript. Dokumentiere Rundungen und relevante Toleranzen bzw. Unsicherheiten.
- Lege, soweit relevant, Bezugsrichtungen und Vorzeichen, Effektiv- und Scheitelwerte, Frequenz und Kreisfrequenz sowie Amplituden- und Leistungsgrößen bei Dezibelangaben offen. Unterscheide ideale Modelle von realen Bauteilen, einschließlich ihrer Gültigkeitsgrenzen.
- Kennzeichne didaktisch gewählte Ausgangswerte als Annahmen. Trenne analytische Rechnung, Simulation und Messung; behaupte eine Ausführung oder ein Messergebnis nur, wenn sie tatsächlich vorliegt.

### Modellannahmen und fachliche Prüfungen

- Spezifiziere für Ausführungsbeispiele Eingaben, erwartete Ausgaben, relevante Anfangszustände und Ereignisreihenfolgen. Benenne nötige Annahmen zu Zahlendarstellung, Sprache, Architektur und Ausführungsmodell. Definiere bei gleichzeitig eintretenden Ereignissen die verwendete Reihenfolge, soweit sie das Ergebnis beeinflusst.
- Unterscheide Programmiersprachensemantik, logisches Modell, Befehlssatz, Mikroarchitektur und konkrete Hardware. Leite Aussagen über eine Ebene nur mit passenden Annahmen und Nachweisen auf eine andere über. Kennzeichne idealisierte Modelle und ihre Gültigkeitsgrenzen.
- Halte Darstellungen konsistent: Prüfe kleine Schaltnetze möglichst über alle Eingangskombinationen, Automaten über Zustandsübergänge und Beispielsequenzen, Programme über Normal- und Randfälle und Scheduling-Diagramme gegen die zugrunde liegende Rechnung. Nutze die [fachabhängigen Prüfkriterien](FACHREGELN.md); beobachtete Einzelabläufe ersetzen keinen allgemeinen Korrektheits- oder Zeitnachweis.
- Unterscheide Taktzyklen, Timer-Ticks, Prozessorzeit und reale Zeit. Lege für Umrechnungen Taktquelle, Zählbereich, Vorteiler und Rundungen offen, soweit relevant. Leite aus Quellcodezeilen oder einer bloßen Befehlsanzahl keine allgemeine Laufzeit ab.
- Erkläre bei Nebenläufigkeit Race Conditions, Deadlocks, Verhungern und Prioritätsinversion, soweit behandelt, mit nachvollziehbaren Ausführungsfolgen. Benenne bei Gegenmaßnahmen die gewährleistete Eigenschaft und deren Voraussetzungen; unterscheide Sichtbarkeit von Speicherzugriffen, Atomizität und Synchronisation. `volatile` gewährleistet keine Atomizität und keine allgemeine Synchronisation.

### Übungs- und Laboraufgaben

- Benenne Lernziel, Voraussetzungen, benötigte Werkzeuge und einen realistischen Bearbeitungsumfang. Aufgaben müssen mit den angegebenen Informationen und Hilfsmitteln lösbar sein.
- Beschreibe bei Messaufgaben Schaltung bzw. Messaufbau, Bauteilwerte, Bezugsrichtungen, Geräteeinstellungen, Messgrößen und Auswertung. Gib erwartete Beobachtungen oder begründete Wertebereiche an und berücksichtige relevante Messunsicherheiten.
- Dokumentiere für Simulationen und Softwareversuche die nötigen Modellannahmen, Versionen, Parameter und Arbeitsschritte. Bewahre verwendete Skripte und zulässige Eingabedaten nachvollziehbar im Repository auf.
- Erstelle zu jeder Aufgabe eine überprüfte Musterlösung mit Rechenweg bzw. Auswertung und typischen Fehlannahmen. Halte Lösungen in Übungsblättern im vorgesehenen `solution`-Profil und prüfe, dass sie in der Studierendenversion nicht erscheinen.
- Bei Programmieraufgaben sind nummerierte TODOs und ein ausdrücklich dokumentierter unvollständiger Startzustand beabsichtigt. Erhalte die studentischen Ergänzungsstellen; löse sie nicht automatisch beim Validieren. Aufgabenstellung, Startcode, Referenzlösung und Tests müssen dieselben Schnittstellen und Annahmen verwenden. Paketdateien und erwartete Prüfresultate stehen in `code/labs/<lab_id>/lab.json`.

## Tech-Stack und Quarto-Konventionen

Gilt für Quarto-, Markdown- und Stylesheet-Dateien (`**/*.{qmd,scss,css,md}`).

### Tech-Stack

- **Framework:** Quarto (Websites, PDFs, Reveal.js)
- **Build-Umgebung:** Gemeinsames, versionsgebundenes Setup in `scripts/setup.sh`; vor Renderläufen `source scripts/activate.sh` ausführen. Unterstützte Plattformen, Systemabhängigkeiten und Werkzeugversionen stehen in [TEMPLATE.md](TEMPLATE.md#build-umgebung-einrichten). Nutze dieselben Werkzeuge für Cloud und CI; installiere oder aktualisiere Browser, Node oder TeX-Pakete nicht unabhängig davon.
- **Sprachen:** Markdown (`.qmd`), SCSS; ggf. modulspezifischer Quellcode und Python für Hilfsskripte
- **Diagramme:** Werkzeug nach dem fachlichen Inhalt wählen, siehe [Diagramm-Entscheidungsbaum](#diagramm-entscheidungsbaum). Mermaid nativ in Quarto; WaveDrom/PlantUML über Extension `seandavi/quarto-livefigures` (Filter in `_quarto.yml`, Node ≥ 18).
- **Kodierung:** `.qmd` als UTF-8 ohne BOM speichern; die [Folienregeln](vorlesungen/AGENTS.md#utf-8-bei-folien-edits) beschreiben Prüfung und Reparatur.

### Quarto / Markdown

- Jede neue Folie im Dokumentkörper mit `## Titel` (Level-2-Header) beginnen. `---` nur für das YAML-Frontmatter verwenden, nicht als Folientrenner: Der aktuelle PDF-Handoutfilter erkennt ausschließlich Überschriften als Foliengrenzen.
- Begleitende Lernnotizen **immer** in `::: notes` … `:::` schreiben. Sie erklären die Folie als zusammenhängenden Lerntext und dürfen didaktische Hinweise enthalten; reine Regieanweisungen reichen nicht aus.
- **Übungsblätter** unter `labs/lab_NN_….qmd`: Lösungen in `::: {.content-visible when-profile="solution"} … :::`; Studierende ohne Profil, Musterlösung mit `--profile solution` (siehe `labs/_quarto-solution.yml`, `build_labs.sh`)
- Code-Blöcke immer mit passender Sprache taggen für Syntax-Highlighting

### Ausgabeformate und Prüfung

Verwende nach Einrichtung und Aktivierung der gemeinsamen Build-Umgebung `python3 scripts/build-materials.py all` als vollständige Prüfung für Cloud und CI. Der Build muss für alle vorhandenen passenden Quellen Website, PDF-Handouts sowie Studierenden- und Lösungsversionen der Übungsblätter erfolgreich erzeugen. Vorhandene Programmierprojekte werden zusätzlich geprüft und als Startcode-ZIP paketiert. Für gezielte Arbeitsstände sind die Teilbefehle `website`, `handouts`, `labs` und `code` verfügbar; `labs` und `bash build_labs.sh` prüfen auch die zugehörigen Codeprojekte.

| Zweck | Aufruf aus dem Repository-Stamm | Erwartete Ausgabe |
|-------|--------------------------------|-------------------|
| Website und Vorträge mit Notizen nur in der Speaker View | `python3 scripts/build-materials.py website` | u. a. `_site/vorlesungen/01_was-ist-echtzeit.html` |
| Reguläre PDF-Handouts mit Folieninhalt und Lernnotizen | `python3 scripts/build-materials.py handouts` | u. a. `_site/vorlesungen/01_was-ist-echtzeit-handout.pdf`; zusätzlich `_handout/vorlesungen/01_was-ist-echtzeit.pdf` |
| Übungsblätter und Musterlösungen | `python3 scripts/build-materials.py labs` | u. a. `_site/labs/lab_01_bare-metal-polling-interrupts.pdf` und `lab_01_bare-metal-polling-interrupts-musterloesung.pdf`; zusätzlich `labs/_output/*-{studierende,musterloesung}.pdf` |
| Programmierprojekte einschließlich Startzustand, Referenzlösung und entpacktem Paket | `python3 scripts/build-materials.py code` | `_site/labs/<lab_id>-startcode.zip` für vorhandene Manifeste |
| Optionales HTML-Handout mit sichtbaren Notizen | `quarto render vorlesungen/01_was-ist-echtzeit.qmd --profile handout --to revealjs` | `_handout/vorlesungen/01_was-ist-echtzeit.html` |

Der gemeinsame Build verarbeitet Vorlesungen und nummerierte Übungsblätter bzw. `probeklausur.qmd` auch in Unterordnern, schließt Vorlagen aus und übernimmt nur frisch erfolgreich gerenderte Ergebnisse. Verwende alte PDFs im Quellordner nicht als Erfolgsnachweis. Fehlende oder leere Pflichtausgaben und Renderfehler dürfen nicht übersprungen werden; ein fehlgeschlagener Build darf keine Veröffentlichung auslösen. Die Veröffentlichung verwendet ausschließlich die geprüften Dateien aus `_site/`.

Verwende für neue Vorlesungen standardmäßig Vortrag und reguläres PDF-Handout. Das zusätzliche Erweiterungsformat `handout-revealjs` ist eine abweichende Variante; seine Einstellungen nicht mit dem Profil `handout` vermischen. A4-Hochformat und Notizdarstellung werden im jeweiligen Handout konfiguriert. Layoutvorgaben für den Vortrag stehen unter [Reveal.js-Leinwand](#revealjs-leinwand).

Rendere nach inhaltlichen oder Layoutänderungen die betroffenen Ausgabeformate. Prüfe Zuordnung und Vollständigkeit der Notizen, lesbare Formeln und Beschriftungen sowie abgeschnittene Inhalte. Für Übungsblätter beide Profile prüfen; ein erfolgreicher Website-Build ersetzt weder die PDF-Prüfung noch die fachliche Kontrolle. Berichte die tatsächlich ausgeführten Prüfungen und verbleibende Grenzen.

## Diagramm-Entscheidungsbaum

Gilt für Diagramme in Quarto-, Markdown- und SVG-Dateien (`**/*.{qmd,md,svg}`).

Wähle das Werkzeug nach dem fachlichen Inhalt. Mermaid ist der Standard für qualitative Struktur- und Ablaufdiagramme; quantitative Kurven, elektrische Schaltpläne und präzises Zeitverhalten benötigen dafür geeignete Darstellungen.

### Werkzeugwahl

| Inhalt | Werkzeug | Einbindung in Quarto |
|--------|----------|----------------------|
| Digitale Signale, Clocks, Busprotokolle, Flanken, Interrupt-Latenzen | **WaveDrom** | `` ```{.wavedrom} `` |
| Timing-/Scheduling-Diagramme: Task-Zustände, Präemption, Deadlines, Ressourcen | **PlantUML** | `` ```{.plantuml} `` |
| Zustandsautomaten, Architektur, Datenfluss, qualitative Abläufe und Aufrufsequenzen | **Mermaid** | `` ```{mermaid} `` |
| Kennlinien, Zeitverläufe, Spektren, Bode- und Ortskurven | Berechnete Plots, z. B. mit Python/Matplotlib oder einem vorhandenen Fachwerkzeug | Exportierte SVG-, PDF- oder PNG-Datei, passend zum Ausgabeformat |
| Elektrische Schaltpläne und analoge Schaltungen | Geeignetes Schaltplanwerkzeug oder fachlich korrektes SVG mit Schaltsymbolen nach IEC 60617 | Grafik unter `assets/images/`, bearbeitbare Quelle erhalten |
| Logikschaltungen und Gatter-Layouts | **SVG** mit Logiksymbolen nach IEC 60617 | Siehe [SVG-Workflow](#workflow-logikschaltungen--svg-generierung) |

**Verbot:** Mermaid nicht für präzises taktebene Hardware-Timing oder detailliertes präemptives Scheduling verwenden — dafür WaveDrom bzw. PlantUML.

### Wissenschaftliche Plots und Schaltpläne

- Berechne quantitative Kurven aus dokumentierten Gleichungen, Parametern oder vorhandenen Daten. Versioniere das Erzeugungsskript und zulässige Eingabedaten, etwa unter `code/`; exportiere die Abbildung nach `assets/images/`. Kennzeichne frei gewählte Beispielparameter und schematische Verläufe.
- Beschrifte Achsen mit Größe und Einheit, benenne lineare bzw. logarithmische Skalierung und lege relevante Bezugsgrößen und Vorzeichenkonventionen offen. Prüfe Wertebereich, Auflösung, Grenzfälle und Übereinstimmung mit der zugehörigen Rechnung. Nutze Legenden und unterscheidbare Linienarten.
- Zeige in Schaltplänen eindeutige Verbindungen, Bezugspotenziale, Bauteilbezeichnungen und relevante Werte. Unterscheide eine elektrische Schaltung von einem funktionalen Blockdiagramm; prüfe die fachliche Funktion zusätzlich zum grafischen Layout.

### Syntax (kritisch)

- WaveDrom/PlantUML: Klassenform **mit Punkt** — `{.wavedrom}` / `{.plantuml}`. Ohne Punkt (`{wavedrom}`) behandelt Quarto das als Engine und bricht ab.
- Mermaid: Engine-Klammern **ohne** führenden Punkt — `{mermaid}` (nativ in Quarto).
- Extension: `seandavi/quarto-livefigures` (Filter `livefigures`, `pre-ast`). WaveDrom offline (Node ≥ 18); PlantUML über Kroki (`https://kroki.io`, Netzwerk bei Cache-Miss).

### WaveDrom

- Valides WaveJSON generieren; Zeilen kurz halten (kein Folienüberlauf).
- Markierungen/Pfeile sparsam; Akzente in DHBW-Rot `#E2001A`.

### PlantUML (Timing)

- Timing-Diagramme mit `concise` und/oder `robust` (PlantUML-Timing).
- **Immer** diese `skinparam` am Anfang des Blocks (Apple/DHBW-Look):

````markdown
```{.plantuml}
@startuml
skinparam backgroundColor transparent
skinparam defaultFontName "Inter, sans-serif"
skinparam defaultFontColor #1D1D1F
skinparam ArrowColor #656D76
skinparam NoteBackgroundColor #F5F5F7
skinparam NoteBorderColor #656D76
skinparam timing {
  BackgroundColor #FFFFFF
  BorderColor #656D76
  LineColor #1D1D1F
  SignalColor #E2001A
}
' … Timing-Inhalt …
@enduml
```
````

### Mermaid & SVG

- Mermaid-Styling: siehe [Design-Richtlinien](#design-richtlinien) / [Tech-Stack und Quarto-Konventionen](#tech-stack-und-quarto-konventionen) (Fence `{mermaid}`, kein manuelles `fontSize`, DHBW-Rot mit `!important`).
- Logikschaltungen / SVG-Workflow: siehe Abschnitt in [Design-Richtlinien](#design-richtlinien).

## Design-Richtlinien

Gilt für Folien, Website, Stylesheets und SVGs (`**/*.{scss,css,qmd,svg}`).

Ästhetik: minimalistisch, modern („Apple-Look“ / Jony Ive).

- Viel Whitespace, weiche Schatten und abgerundete Ecken; dekorative Rahmen zurückhaltend gestalten, fachlich notwendige Konturen deutlich zeichnen
- Typografie: Sans-Serif (Inter / San Francisco), Zeilenabstand 1.4 auf Folien

### Farben und Lesbarkeit

Lesbarkeit hat Vorrang vor dekorativen Farbvorgaben. Verwende die folgende Basispalette; passe vorhandene Design-Tokens und Diagramme bei Bedarf unter Beachtung der Kontrastanforderungen an.

| Rolle | Wert | Verwendung |
|-------|------|------------|
| Hintergrund Folien | `#FFFFFF` | Reveal.js |
| Hintergrund Website | `#FFFFFF` | Quarto-Site |
| Abgesetzte Flächen | `#F5F5F7` | Code- und Notizblöcke |
| Haupttext | `#1D1D1F` | Body |
| Sekundärtext und tragende Linien | `#656D76` | Captions, Beschriftungen, Gatterkonturen |
| Dekorative Linien | `#E5E5EA` | Nur ohne inhaltstragende Funktion |
| Akzent DHBW Rot | `#E2001A` | Nur sparsam: Links, Highlights, Fortschrittsbalken, Mermaid-Akzente |

- Normal großer Text benötigt mindestens 4,5:1 Kontrast zum Hintergrund; große Schrift und für das Verständnis notwendige Linien, Symbole und Konturen mindestens 3:1 zur angrenzenden Fläche. Prüfe die tatsächlich verwendeten Vorder- und Hintergrundfarben. `#86868B` auf Weiß erreicht nur etwa 3,62:1, `#E5E5EA` etwa 1,26:1; diese Farben sind daher nicht für kleine Beschriftungen bzw. tragende Konturen geeignet.
- Wähle Schrift und Strichstärken so, dass sie auch auf dem Beamer und im PDF lesbar bleiben. Nutze zusätzliche Farben für Datenreihen nur gezielt und mit ausreichendem Kontrast. Unterscheide Zustände und Kurven zusätzlich durch Beschriftungen, Symbole oder Linienarten; Farbe allein darf keine Information tragen.
- Ergänze für Abbildungen fachlich aussagekräftige Alternativtexte bzw. Beschreibungen. Prüfe zentrale Aussagen auch in Graustufen und behalte zugänglichen Text statt ausschließlich grafischer Beschriftungen bei.

### Typografie auf Folien

- Kein flächiges `**Label:**`-Bolding in Bullet Points (wirkt „laut“ und erschwert das Lesen)
- Listen standardmäßig in normaler Schriftstärke; Bold nur sparsam für echte Kontraste (max. 0–1 Hervorhebung pro Folie)
- Überschriften (`#` / `##`) behalten `font-weight: 600`

### Platzknappheit & Layout

Reihenfolge bei Overflow (Titel + Bullets + Diagramm/Code):

1. Inhalt sinnvoll auf mehrere Folien aufteilen, kürzen oder Diagramm vereinfachen
2. Ausrichtung dem Inhalt anpassen; bei breiten Ketten etwa `graph TD` statt `graph LR` prüfen
3. Gezielt Zweispaltigkeit (`:::: {.columns}`) nutzen, wenn alle Elemente lesbar bleiben

Zweispaltigkeit ist die **Ausnahme**, nicht der Standard:

- Links: max. 3–5 kurze Bullets
- Rechts: kurzer **Code**, kompakte **Tabelle** oder ausreichend großes **Diagramm**
- Mermaid ist auch in Spalten zulässig; prüfe die korrekte Fence-/Div-Verschachtelung und das gerenderte Ergebnis. Nutze bei Platzmangel die volle Breite oder eine eigene Folie.
- Schrift nur leicht kleiner, wenn nötig — Hörsaal-Lesbarkeit beachten

`{.scrollable}` ist eine optionale Hilfe für bewusst interaktive HTML-Folien. Behebe Überladung bevorzugt durch Aufteilen; Scrollen ersetzt keine vollständige, lesbare Darstellung im Vortrag und PDF-Handout.

### Diagramm-Werkzeuge

Zuerst Entscheidungsbaum in [Diagramm-Entscheidungsbaum](#diagramm-entscheidungsbaum) (WaveDrom / PlantUML / Mermaid / SVG). Mermaid-Styling nur, wenn Mermaid gewählt wurde:

- Fence immer mit Engine-Klammern: ` ```{mermaid} ` (ohne `{}` wird Mermaid oft als Codeblock gerendert)
- **Keine** manuelle `fontSize` über `%%{init: …}%%` — Skalierung nur über SCSS / `mermaid-fit`
- Farben und Konturen nach [Farben und Lesbarkeit](#farben-und-lesbarkeit); helle Flächen und dekorative Linien dürfen keine notwendigen Elemente unsichtbar machen.
- Folien: SVG-Overrides unter `.reveal .mermaid` / `.mermaid-fit` in `slides.scss` — **kein** globales `max-height`
- Didaktische Hervorhebungen im Markdown mit DHBW-Rot und **immer** `!important`:
  `style [Knoten] fill:#FFFFFF,stroke:#E2001A,stroke-width:2px !important;`

### Reveal.js-Leinwand

- Reveal.js-Leinwand: `auto-stretch: false` in `_quarto.yml` — sonst setzt Quarto `r-stretch` auf das letzte Bild und SVG-Schaltungen kollabieren oft auf Höhe 0 (unsichtbar).
- Für den **Vortrag** gelten `center: false`, 16:9 (`1280`×`720`) und `margin: 0.08`. Eigene Folien dürfen diese Vorgaben nicht ohne bewusste Layoutentscheidung überschreiben.
- Für **Handouts** gelten die Einstellungen des jeweiligen Profils bzw. Formats; das optionale HTML-Handout verwendet derzeit `center: true`. Prüfe dessen Layout separat und übertrage diese Einstellung nicht pauschal auf den Vortrag.

#### Workflow: Logikschaltungen & SVG-Generierung

- **Trigger:** Wenn der User nach einer "Logikschaltung", einem "Gatter-Layout" oder dem "Zeichnen einer Schaltung" fragt, generierst du NIEMALS Mermaid-Code, sondern schreibst IMMER direkt eine saubere, rohe `.svg`-Datei in den Ordner `assets/images/`.
- **DIN-Norm (IEC 60617):** Verwende ZWINGEND die europäische Norm für Schaltsymbole.
  - Logikgatter sind strikt rechteckige Boxen (`<rect>`).
  - Text-Symbole zentriert in der Box: AND = `&`, OR = `>=1`, zweieingängiges XOR = `=1`, Buffer = `1`.
  - Bei mehr als zwei Eingängen ungerade Parität und Genau-eins-Funktion ausdrücklich unterscheiden; `=1` nicht pauschal für eine Paritätsfunktion verwenden. Siehe [Digitaltechnik](FACHREGELN.md#digitaltechnik).
  - Negationen (z. B. bei NAND, NOR, NOT) werden NICHT durch amerikanische Formen, sondern durch einen kleinen Kreis (`<circle>`) direkt am Ausgang der Rechtecke gezeichnet.
- **Styling (DHBW/Apple):**
  - Gatter-Hintergrund: `#FFFFFF`
  - Gatter-Rahmen (Stroke): `#656D76` (stroke-width: mindestens 2 im vorgesehenen Maßstab)
  - Verbindungslinien und Pfeile: `#656D76`
  - Text (Labels & Symbole): `#1D1D1F`, Schriftart `Inter, sans-serif`
  - Lege am Anfang des SVGs einen `<style>`-Block für diese Farben an.
  - XML: Das AND-Symbol **immer** als `&amp;` escapen (`<text>&amp;</text>`), sonst fehlt das Zeichen in PDF-Konvertern (svglib/rsvg).
- **Einbindung in Folien:** `![Beschreibung](../assets/images/dateiname.svg)` — kein Mermaid; bei Bedarf explizit `{.nostretch}`.
- **Agentic Self-Correction & Browser-Check (PFLICHT):**
  1. Berechne die X/Y-Koordinaten für Boxen und Pfade (`<path>`) systematisch. Führe Linien immer rechtwinklig (orthogonal).
  2. Speichere das generierte SVG zunächst temporär ab.
  3. Nutze deine internen Browser-/Terminal-Tools, um das SVG zu validieren. Prüfe logisch, ob sich `<rect>`-Koordinaten überlappen oder Linien quer durch Boxen laufen.
  4. Korrigiere die Koordinaten iterativ, bis das Layout sauber und lesbar ist. Vermeide unbeabsichtigte Kreuzungen; kennzeichne notwendige Leitungskreuzungen und Verbindungspunkte eindeutig. Binde das finale SVG in eine Folie unter `vorlesungen/` als `![Beschreibung](../assets/images/dateiname.svg)` ein; passe den relativen Pfad bei anderen Quellverzeichnissen an.
