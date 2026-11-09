# Gemeinsame Arbeitsweise der Projekt-Skills

Diese Arbeitsweise ergänzt die [Projektregeln](../../../AGENTS.md); die passenden Abschnitte aus [FACHREGELN.md](../../../FACHREGELN.md) sowie die Regeln des bearbeiteten Unterverzeichnisses bleiben maßgeblich. Pfade in den Skill-Anleitungen sind relativ zur jeweiligen `SKILL.md`, Shell-Befehle werden aus dem Repository-Stamm ausgeführt. Verwende den vorhandenen Checkout; erstelle keinen zusätzlichen Git-Worktree ohne ausdrücklichen Wunsch.

## Kontext beschaffen

Lies zuerst den Auftrag, `README.md` (Kursrahmen und Lernziele), `_quarto.yml` und die vorhandenen Materialien im angefragten Bereich. Übernimm bereits bekannte Angaben und Präferenzen aus der Sitzung. Wähle Fachregeln nach dem tatsächlichen Inhalt, auch fachübergreifend.

Lies vor dem Erstellen oder Überarbeiten das [Kursglossar](../../../GLOSSAR.md) und die zugehörigen Folien, Lernnotizen, Übungen, Musterlösungen, Quizfragen und Codebeispiele im betroffenen Themenbereich. Stimme die Begriffswahl materialübergreifend nach den [Terminologieregeln](../../../AGENTS.md#präzise-terminologie) ab: eingeführte Vorzugsbegriffe übernehmen, Synonyme ausdrücklich zuordnen und echte API-Bezeichner erhalten sowie ihrem fachlichen Konzept zuordnen. Fehlen Glossar oder relevante Einträge, ergänze sie bei einem Bearbeitungsauftrag im bestehenden Umfang konsistent aus den verfügbaren Quellen; benenne widersprüchliche Bezeichnungen und offene Zuordnungen, statt sie stillschweigend zu vereinheitlichen. Ein reines Review meldet solche Lücken als Befund.

| Auftrag | Zusätzlich benötigte Angaben, soweit nicht schon dokumentiert |
|---------|-------------------------------------------------------------|
| Kurs planen | Modulvorgaben bzw. verfügbare Quellen, Vorkenntnisse, Umfang und vorgesehene Prüfungsform |
| Einheit erstellen | Thema und Lernziele, Einordnung in den Kurs, Zeitbudget und Zielpfad bzw. betroffene Einheit |
| Übungsblatt erstellen | Lernziele und zugehörige Einheit, gewünschter Umfang, Hilfsmittel und gegebenenfalls Zielhardware |
| Quiz erstellen | Zugehörige Vorlesungsquellen, Lernziele, gewünschte Fragenzahl bei neuen Dateien und Zielpfad |
| Material prüfen | Betroffene Dateien bzw. Einheiten und Prüfschwerpunkt; ohne Schwerpunkt die relevanten fachlichen und didaktischen Kriterien verwenden |
| Ausgaben validieren | Betroffener Bereich bzw. vollständiger Kurs und verfügbare Build-Werkzeuge |

Klär nur Angaben nach, deren Fehlen eine fachlich belastbare Bearbeitung verhindert, etwa die Wortbreite einer davon abhängigen Rechenaufgabe. Fahre mit unabhängigen Arbeitsschritten fort. Kennzeichne vertretbare didaktische Annahmen und offene Punkte im Entwurf bzw. Prüfbericht; erfinde keine offiziellen Vorgaben, Quellen, Hardwareeigenschaften oder erfolgreichen Werkzeugläufe. Ein fehlendes Detail ist kein Grund für eine pauschale neue Abfragerunde über bereits dokumentierte Kursdaten.

Für Aufgabenentwurf und -review gelten die Regeln zu [Verständnis, Transfer und Prüfungsvorbereitung](../../../AGENTS.md#verständnis-transfer-und-prüfungsvorbereitung). Beurteile den eigenständigen Denkschritt am Lösungsweg und an den zugehörigen Lehrmaterialien. Begründe Schwierigkeit und Bearbeitungszeit zunächst als Schätzung anhand von Vorkenntnissen, Denk-/Rechenschritten und Hilfen; nutze tatsächliche Bearbeitungserfahrungen und Rückmeldungen zur späteren Kalibrierung. Halte die Autorenprüfung zu Lernziel, Denkschritt, Hilfen und Prüfungsbezug kompakt in vorhandenen Notizen oder im Abschlussbericht fest; dafür sind keine zusätzlichen Dateien oder Metadatenformate erforderlich.

## Überarbeiten und Auftragsgrenzen

- Erhalte standardmäßig vorhandene Struktur, Inhalte, Dateinamen, Nummerierung und Verweise. Setze die beauftragten Änderungen im betroffenen Bereich vollständig um und sichere die Konsistenz abhängiger Stellen.
- Schlage größere didaktische Umstrukturierungen mit Begründung und Auswirkungen vor. Führe sie aus, wenn der Nutzer sie beauftragt hat; bereits erteilte Anweisungen brauchen keine erneute Bestätigung. Eine neue Einheit ohne bestehende Struktur wird passend zu ihren Lernzielen entworfen.
- Ein Review liefert standardmäßig Befunde. Eine ausdrücklich mitbeauftragte Korrektur wird im autorisierten Umfang umgesetzt. Automatische Formatprüfungen ersetzen keine fachliche oder didaktische Prüfung.
- Commit, Push, Pull Request und Veröffentlichung gehören nur zum Ablauf, wenn der aktuelle oder weiter geltende Sitzungsauftrag sie umfasst. Der Skill selbst führt keinen zusätzlichen Freigabeprozess ein.

## Quellen, Diagramme und technische Prüfungen

Verweise auf die zentralen Fach- und Designregeln statt eigene abweichende Kopien anzulegen. Für Diagramme gilt der Entscheidungsbaum aus `AGENTS.md`, einschließlich der IEC-SVG-Vorgaben. Der [mitgelieferte livefigures-Skill](../../../_extensions/seandavi/livefigures/SKILL.md) kann als technische Referenz für die Extension dienen; seine allgemeinen Werkzeugpräferenzen und das Verbot manuell platzierter Koordinaten ersetzen die projektspezifischen Vorgaben nicht. Installiere die bereits enthaltene Extension nicht erneut, um einem generischen Beispiel zu folgen.

Nutze die gemeinsame Toolchain und die vorhandenen Build-Skripte. Für technische Validierung beschreibt [material-validieren](../material-validieren/SKILL.md) die Prüfungen; dortige Befehle können direkt verwendet werden, auch wenn das KI-Tool keine verschachtelte Skill-Aktivierung unterstützt. Ein nicht verfügbares Werkzeug oder ein fehlgeschlagener Build wird konkret berichtet. Unterscheide durchgeführte, fehlgeschlagene und nicht ausgeführte Prüfungen sowie deren Aussagegrenzen.

## Ergebnisse übergeben

Nenne die erzeugten bzw. geänderten Quellen, den fachlichen und didaktischen Zweck, tatsächlich durchgeführte Prüfungen und wesentliche verbleibende Unsicherheiten. Bei Planung oder Review sind Plan bzw. Befunde das Ergebnis; behaupte keine Erstellung oder Prüfung weiterer Kursmaterialien. Überschreibe keine fremden Änderungen und behaupte keinen erfolgreichen CI-Lauf allein aufgrund eines lokalen Builds.
