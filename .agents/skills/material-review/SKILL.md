---
name: material-review
description: Vorlesungen, Lernnotizen, Laboraufgaben und Quizfragen dieses DHBW-Quarto-Templates fachlich und didaktisch prüfen; belegte, priorisierte Befunde und konkrete Verbesserungsvorschläge liefern, ohne standardmäßig Dateien zu ändern.
---

# Lehrmaterial prüfen

## Auftrag und Eingaben

Nutze diesen Skill bei einem Review, einer Qualitätskontrolle oder der Frage,
ob vorhandenes Material fachlich und didaktisch trägt. Ein Review verändert keine
Dateien, solange der Auftrag keine Umsetzung der Korrekturen einschließt.

Ermittle die betroffenen Dateien, das gewünschte Prüfniveau und den Kursrahmen
aus dem Auftrag und `README.md`. Nutze vorhandene Lernziele, Zielgruppe,
Zeitbudget, Prüfungsform, Literatur und technische Modellannahmen.
Fehlende Angaben nur dann erfragen, wenn sie eine belastbare Bewertung verhindern;
sonst als konkrete Annahme oder offene Frage im Bericht kennzeichnen.

Lies die [gemeinsame Arbeitsweise](../references/arbeitsweise.md), die
[Projektregeln](../../../AGENTS.md) und die einschlägigen
[Fachregeln](../../../FACHREGELN.md). Bei Folien gelten zusätzlich die
[Vorlesungsregeln](../../../vorlesungen/AGENTS.md), bei Quizfragen die
[Quizregeln](../../../quizacademy/AGENTS.md).
Bei Programmieraufgaben außerdem die [Code-Regeln](../../../code/labs/AGENTS.md)
und das Manifest sowie README, Startcode, Referenzlösung und Tests des Projekts lesen.
Das [Kursglossar](../../../GLOSSAR.md) als Grundlage des Terminologieabgleichs lesen.

## Vorgehen

1. **Umfang abgrenzen.** Prüfe die vereinbarten Quellen und nötige Abhängigkeiten
   wie Abbildungen, Code, Lösungen und Kurskontext. Nenne ausgeschlossene
   Bereiche. Bei Änderungen auch den fachlich relevanten Kontext der Änderung lesen.
2. **Lernweg prüfen.** Passen Lernziele, Voraussetzungen, Begriffsaufbau, Beispiele,
   Übungen und Zeitbudget zusammen? Sind neue Begriffe erklärt und die vorhandenen
   Notizen ein zusammenhängender, mit sichtbarer Folie verständlicher Lerntext? Jede Folie zählt,
   einschließlich Titel, Orientierung und Abschluss.
   Prüfe bei Vorlesungen die [aktivierende Einstiegsfrage](../../../vorlesungen/AGENTS.md#aktivierende-einstiegsfrage-je-vorlesung) ausdrücklich:
   Platzierung vor Agenda/Lernzielen, Passung zum Vorwissen, begründbare Antworten ohne vorweggenommene Lösung,
   Denk-/Austauschphase, vollständige Auflösung samt Fehlvorstellungen in den Notizen und spätere Wiederaufnahme.
   Prüfe je Aufgabe Lernziel und eigenständigen Denkschritt nach den [Didaktikregeln](../../../AGENTS.md#verständnis-transfer-und-prüfungsvorbereitung).
   Belege Foliennähe sowie Unter- oder Überforderung konkret am Lösungsweg und an den zugehörigen Folien/Beispielen mit Fundstellen;
   benenne dafür die vorausgesetzten Kenntnisse, Schritte, Hilfen und geschätzte Zeit und schlage eine passende Aufgabenvariante vor.
   Beurteile Staffelung und Prüfungsvorbereitung anhand der tatsächlichen Prüfungsanforderungen; fehlende Vorgaben oder Rückmeldungen begrenzen die Bewertung.
3. **Darstellungen abgleichen.** Vergleiche Folientext, Notizen, Formeln, Code,
   Abbildungen, Aufgaben, Musterlösungen und zugehörige Quizfragen. Gleiche die Terminologie
   dieser Quellen ausdrücklich mit dem Kursglossar und den [Terminologieregeln](../../../AGENTS.md#präzise-terminologie) ab.
   Benenne abweichende Bezeichnungen, nicht zugeordnete Synonyme und Konzept-/API-Verwechslungen mit den jeweiligen Fundstellen.
   Prüfe Annahmen, Eingaben,
   Anfangszustände, Einheiten, Rundungen, Grenzfälle und Quellenfundstellen.
4. **Fachprüfung auswählen.** Wende nur passende Kriterien aus der folgenden
   Liste an; fachübergreifende Beispiele können mehrere Bereiche betreffen.
5. **Nachweise bewerten.** Prüfe vorhandene Rechnungen und zulässige Quellen.
   Kleine unabhängige Gegenrechnungen oder temporäre Prüfläufe sind möglich, wenn
   Eingaben und Modell bekannt sind und sie keine Projektdateien verändern.
   Beschreibe tatsächlich verwendete Werte, Werkzeuge und Grenzen.
6. **Befunde formulieren.** Berichte zuerst Fehler mit Folgen für Verständnis,
   Lösbarkeit oder Korrektheit, danach didaktische Lücken und kleinere Verbesserungen.
   Erhalte die vorhandene Struktur und Inhalte. Größere Umstellungen als
   begründeten Vorschlag ausweisen, sofern sie nicht bereits beauftragt sind.

## Fachabhängige Prüfungen

- **Informatik:** Sprachstandard und Laufzeit, Ein-/Ausgabevertrag, Randfälle,
  Invarianten, Terminierung und Komplexitätsmodell prüfen. Definiertes,
  implementierungsabhängiges, nicht spezifiziertes und undefiniertes Verhalten
  unterscheiden; bei C/C++ mit undefiniertem Verhalten keine eindeutige Ausgabe
  behaupten. Ein erfolgreicher Test allein beweist keine allgemeine Korrektheit.
- **Digitaltechnik:** Wortbreite, Basis, Vorzeichen und Bitreihenfolge prüfen.
  Kleine Wahrheitstabellen vollständig mit Gleichung und Schaltung abgleichen;
  bei Automaten Reset, Übergänge und Ausgaben prüfen. Don't-care, X/Z und
  Zeitmodell müssen begründet sein. Bei mehr als zwei Eingängen ungerade Parität
  und Genau-eins-Funktion ausdrücklich unterscheiden.
- **Mikrocomputertechnik:** Konkreten Controller/Kern und Dokumentationsstand
  prüfen. Timer mit Takt, Vorteiler, Zählgrenzen, Rundung und Überlauf nachrechnen;
  Register-Nebenwirkungen, Interrupt-Reihenfolge, Atomizität und Synchronisation
  berücksichtigen. `volatile` allein löst keine Synchronisationsprobleme.
- **Realzeitsysteme:** Taskparameter, CPU-Anzahl, Scheduler, Präemption, Ressourcen
  und Gleichzeitigkeit festlegen. Release, Start, Ende und absolute Deadline trennen;
  Scheduling-Spuren gegen die Rechnung prüfen. Einen endlichen Ablauf ohne
  begründete Abdeckung aller relevanten Fälle nicht als allgemeinen
  Einplanbarkeitsnachweis und einzelne Messwerte nicht als WCET-Beweis werten.
- **Quiz und Labore:** Lösbarkeit, eindeutige richtige Antwort bzw. überprüfbare
  Musterlösung, typische Fehlannahmen und Lernzielbezug prüfen. Studierendenmaterial
  darf keine versteckten Lösungen preisgeben; Quizstruktur gesondert kontrollieren.
- **Programmieraufgaben:** Gleiche Schnittstellen und Annahmen in Blatt, TODOs,
  Startcode, Referenzlösung und Tests prüfen. Beabsichtigte TODOs sind Lernaufgaben,
  keine automatisch zu behebenden Fehler. Bewerte, ob die Referenzlösung das Lernziel
  erfüllt und ob der Startervertrag nur die vorgesehenen offenen Teile zulässt.
  Prüfe die Dateiliste des ZIPs und dessen eigenständige Nutzbarkeit. Erwartete
  Fehlschläge benötigen konkrete Testkennungen bzw. das vereinbarte JSON-Protokoll;
  ein beliebiger fehlgeschlagener Testlauf reicht nicht. Tests dürfen Aufgaben nicht
  durch eine unangeforderte Änderung der Schnittstelle oder Erwartungen umgehen.

## Darstellung und Nachweisgrenzen

Bewerte Lesbarkeit und Notizzuordnung anhand frischer Ausgaben, sofern sie
vorliegen. Für neue Renderläufe [material-validieren](../material-validieren/SKILL.md) verwenden und
dessen ausgeführten Umfang nennen; ohne Ausgabe keine visuelle Freigabe behaupten.
Die Extension-Datei `_extensions/seandavi/livefigures/SKILL.md` dient allenfalls
als Syntaxreferenz. Projektvorgaben zu Mermaid, WaveDrom, PlantUML und IEC-SVG
haben Vorrang vor den generischen Werkzeugempfehlungen der Extension.

## Ergebnis und Abschluss

Jeder Befund nennt Priorität, überprüfte Datei mit Zeile, das konkrete Problem,
seine Auswirkung und einen ausführbaren Korrekturvorschlag. Belege ihn mit
einer Textstelle, Rechnung, einem Gegenbeispiel oder einem tatsächlich geprüften
Ausgabeartefakt; bei Letzterem zusätzlich Folie oder PDF-Seite angeben.
Trenne sichere Fehler, begründete Verbesserungen und offene Fachfragen.
Behaupte Tests, Simulationen oder Hardwareversuche nur, wenn sie tatsächlich erfolgt sind.

Schließe mit geprüftem Umfang, tatsächlich ausgeführten Prüfungen und Grenzen.
Wenn keine Befunde bestehen, nenne das und verbleibende Prüflücken. Der Review
ist abgeschlossen, wenn alle vereinbarten Bereiche bewertet sind; Korrekturen,
Commit, Push, Pull Request und Veröffentlichung folgen daraus nicht automatisch.

Beispielaufrufe: „Prüfe die Einheit zu Interrupts samt Notizen fachlich.“ –
„Prüfe Labor und Musterlösung auf Lösbarkeit und typische Fehlannahmen.“
