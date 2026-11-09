# Programmieraufgaben mit Startcode

Diese Regeln ergänzen die [Projektregeln](../../AGENTS.md) und die jeweils passenden [Fachregeln](../../FACHREGELN.md). Sie gelten für Programmieraufgaben unter `code/labs/` einschließlich Startcode, Referenzlösung, Tests und Paketmetadaten. Der Ablauf ist im Skill [uebungsblatt-erstellen](../../.agents/skills/uebungsblatt-erstellen/SKILL.md) beschrieben.

## Aufbau und Lernziel

- Ordne jedes Projekt über dieselbe Blattkennung zu: `code/labs/lab_NN_thema/` gehört zu `labs/lab_NN_thema.qmd`. Verwende `README.md`, `lab.json`, `starter/`, `solution/` und `tests/` gemäß [technischer Anleitung](../../TEMPLATE.md#programmieraufgaben-mit-startcode).
- Halte Aufgabenstellung, nummerierte TODOs, vorgegebene Schnittstellen und Tests konsistent. Beschreibe in der aufgabenspezifischen README Einrichtung, genaue Befehle, erlaubte Änderungen, erwartete Ergebnisse und den Ausgangszustand. Berücksichtige Bearbeitungszeit und Vorkenntnisse.
- Beabsichtigt unvollständiger Startcode ist für seinen Lehrzweck korrekt. TODOs dürfen offen bleiben und sind bei Review, Build oder allgemeiner Fehlerbehebung nicht eigenmächtig zu lösen. Eine ausdrücklich beauftragte Bearbeitung der Übungsaufgabe darf sie dagegen ausfüllen.
- Startcode muss standardmäßig übersetzbar bzw. importierbar sein. Noch fehlende Funktionen dürfen klar gekennzeichnete Fehler auslösen. Sind Übersetzungsfehler selbst Teil des Lernziels, dokumentiere und prüfe genau diesen abweichenden Ausgangszustand.
- Referenzlösung und Startcode verwenden dieselben Schnittstellen. Die Referenzlösung muss die Aufgabenstellung erfüllen und die vorgesehenen Tests erfolgreich bestehen. Die PDF-Musterlösung muss mit ihr übereinstimmen.

## Werkzeug- und Testvertrag

- Lege Sprache, passende Laufzeit- bzw. Compilerversion und erforderliche Abhängigkeiten fest. Die Rendering-Toolchain ersetzt keine Lehrsprachen-Toolchain. Ergänze bei einer neuen Lehrsprache die tatsächliche Einrichtung in Cloud und CI und berichte nicht verfügbare Prüfungen konkret.
- Deklariere Befehle als Argumentlisten im Manifest. Sie laufen aus `starter/` bzw. `solution/` in isolierten Arbeitskopien, nicht in den versionierten Quellen. Prüfe relevante Normal- und Randfälle.
- Prüfe den erwarteten Starterzustand getrennt von der vollständigen Referenzlösung. Ein erwarteter Rückgabecode ungleich null erfordert ein exakt passendes maschinenlesbares JSON-Ergebnis; ein beliebiger Testfehler oder Prozessabbruch zählt nicht als erwarteter Fehler. Keine pauschale Fehlerunterdrückung verwenden.
- Passe den Erwartungsvertrag nicht an, um einen unerwarteten Defekt als Erfolg umzudeuten. Prüfe bei Abweichungen Aufgabenstellung, TODO-Zuordnung, Referenzlösung und Testergebnis.

## Studierendenpaket

- `student_files` ist die ausdrückliche Liste der auszuliefernden Dateien. Sie enthält die Anleitung, den Startcode, erforderliche Projektdaten und freigegebene Tests. Nimm keine Referenzlösung, internen Tests, Zugangsdaten, Caches oder Buildreste in diese Liste auf.
- Erzeuge das ZIP über den gemeinsamen Build und prüfe die tatsächlich entpackte Datei nochmals mit dem Startervertrag. Alle benötigten Projektdateien müssen enthalten sein; das Paket darf keine Dateien aus dem ursprünglichen Checkout benötigen.
- Das Quarto-Profil `solution` steuert die PDF-Ausgabe. Es verbirgt weder Quellcode noch Referenzlösungen im Git-Repository. In einem öffentlichen Repository ist auch `solution/` öffentlich lesbar; vertrauliche oder später freizugebende Lösungen benötigen eine dafür geeignete getrennte Ablage.
