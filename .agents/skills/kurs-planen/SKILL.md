---
name: kurs-planen
description: Plant ein neues Modul oder überarbeitet dessen Kursrahmen, Lernziele, Einheitenfolge und Zeitbudget anhand des DHBW-Templates.
---

# Kurs planen

## Einsatz und Grundlagen

Verwenden, wenn ein neues Modul aus dem Template eingerichtet oder der didaktische Kursplan überarbeitet wird.
Der Auftrag umfasst Kursrahmen und Planung; konkrete Einheiten oder Übungsblätter nur bei ausdrücklichem Auftrag ausarbeiten.

Vor Beginn lesen:

- [Gemeinsame Arbeitsweise](../references/arbeitsweise.md) und [Projektregeln](../../../AGENTS.md)
- [Kursstartseite](../../../README.md), [Projektkonfiguration](../../../_quarto.yml)
  und [Einrichtungsanleitung](../../../TEMPLATE.md)
- Einschlägige Abschnitte der [Fachregeln](../../../FACHREGELN.md)
- [Kursglossar](../../../GLOSSAR.md) mit bereits eingeführten Begriffen
- Vorhandene Einheiten, Übungen sowie vom Nutzer bereitgestellte Modul- und Prüfungsunterlagen

## Eingaben klären

Zuerst bereits dokumentierte Vorgaben und die vorhandene Kursstruktur auswerten.
Für eine belastbare Planung werden insbesondere benötigt:

- Modulname/-kürzel, Semester/Studienrichtung und vorausgesetzte Kenntnisse
- Verfügbare Präsenz- und Selbstlernzeit, Terminstruktur und Prüfungsform
- Fachschwerpunkte, verbindliche Lernziele, Literatur und Werkzeuge
- Inhaltlich nötige Sprache/Standard, Plattform/Architektur, Toolchain und Zeitmodell

Eine vorhandene Fassung des Modulhandbuchs als Quelle benennen; keine offiziellen Vorgaben erfinden.
Nur fehlende Angaben erfragen, die die Planung wesentlich beeinflussen.
Sonst mit dokumentierten, anpassbaren Annahmen weiterarbeiten; unbekannte offizielle Angaben als offen kennzeichnen.
Für einen ausdrücklich vorläufigen Plan sind Annahmen zulässig, sofern ihre Auswirkungen sichtbar bleiben.

Bei Überarbeitungen bestehende Struktur, Inhalte und funktionierende Anschlüsse erhalten.
Größere didaktische Umstrukturierungen mit Grund und erwarteter Wirkung vorschlagen.
Wenn eine solche Umstrukturierung bereits beauftragt ist, sie vollständig ausarbeiten und nicht erneut freigeben lassen.

## Ablauf

1. **Ausgangslage erfassen.** Kurskontext, vorhandene Lernziele und Inhalte mit tatsächlichen Vorgaben abgleichen.
   Fachliche Lücken, Doppelungen und Abhängigkeiten erfassen; bestehende Nutzeränderungen berücksichtigen.
2. **Kursrahmen dokumentieren.** In `README.md` den Abschnitt „Kursrahmen und Lernziele“ anlegen oder pflegen.
   Verbindliche Vorgaben, didaktische Entscheidungen und offene Annahmen unterscheidbar halten.
   Offene Template-Platzhalter nicht mit erfundenen Kursdaten füllen.
3. **Lernziele formulieren.** Wenige überprüfbare Ziele für das Modul und die einzelnen Einheiten entwickeln.
   Das erwartete Niveau etwa durch „erklären“, „analysieren“, „implementieren“ oder „beurteilen“ konkretisieren.
   Für jedes Ziel geeignete Übungen oder Verständnisprüfungen und einen Bezug zur Prüfungsform vorsehen.
   Prüfungsanforderungen nur aus tatsächlich verfügbaren Vorgaben ableiten; offene Angaben kenntlich machen.
4. **Einheitenfolge planen.** Von Voraussetzungen zu Anwendung und Transfer führen.
   An authentischen IT-/EIT-Anwendungsfällen orientieren, soweit sie zu den Lernzielen passen.
   Pro Einheit Thema, Voraussetzungen, Ziele, Kerninhalte und Übungs-/Prüfidee festhalten.
   Für jede Vorlesung eine [aktivierende Einstiegsfrage](../../../vorlesungen/AGENTS.md#aktivierende-einstiegsfrage-je-vorlesung) aus dokumentiertem Vorwissen oder Alltagserfahrung
   sowie ihre spätere fachliche Auflösung einplanen; Zeit für Denken, Austausch und begründete Antworten berücksichtigen.
   Eigenständige Denkschritte und eine gestufte Übungsfolge nach den [Didaktikregeln](../../../AGENTS.md#verständnis-transfer-und-prüfungsvorbereitung) planen;
   zur Prüfungsvorbereitung Hilfen reduzieren und passende offene Aufgaben ergänzend zu Multiple Choice berücksichtigen.
   Zentrale Begriffe und ihre Einführung den Einheiten zuordnen; das Kursglossar bei Bedarf anhand der vorhandenen Quellen pflegen.
   Fachregeln nach konkreten Inhalten zuordnen; mehrere Fachschwerpunkte dürfen zusammenwirken.
5. **Zeitbudget prüfen.** Erklärung, aktive Bearbeitung, Fragen, Besprechung und Selbststudium berücksichtigen.
   Geplante Zeiten summieren und mit den vorhandenen Stunden abgleichen.
   Bei Überlastung begründete Prioritäten und mögliche Kürzungen benennen; keine zusätzliche Lehrzeit voraussetzen.
6. **Kursdokumentation pflegen.** Die Planung passend in die vorhandene README-Struktur integrieren.
   Eine separate Planungsdatei nur anlegen, wenn sie angefordert ist oder für den Umfang sinnvoll ist; von README verlinken.
   Modulbezüge und Titel in `_quarto.yml` konsistent halten; den README-Include in `index.qmd` erhalten.
   Technische Template-Dokumentation und Theme nicht für Kursinhalte umgestalten.
7. **Materialstatus kenntlich machen.** Bereits vorhandene Quellen und echte Ausgabepfade prüfen.
   Geplante Einheiten als geplant ausweisen; nicht erzeugte Materialien nicht als verfügbar darstellen.
   Für neue Links die bestehenden Ausgabe-Konventionen verwenden, ohne automatisch alle Materialien zu generieren.
8. **Konsistenz prüfen.** Ziele, Abfolge, Voraussetzungen, Übungen, Prüfungsniveau und Zeitrechnung gemeinsam beurteilen.
   Widersprüche unmittelbar beheben und notwendige spätere Entscheidungen ausdrücklich dokumentieren.
9. **Bei geänderter Kursseite rendern.** Die gemeinsame Umgebung aus dem Repository-Stamm aktivieren:

   ```bash
   source scripts/activate.sh
   python3 scripts/build-materials.py website
   ```

   Die Kursstartseite auf Inhalt, Tabellen und Links prüfen; fehlende Einrichtung gemäß technischer Anleitung herstellen.
   Den vollständigen Projektcheck gemäß Projektregeln ausführen, wenn renderbare Kursinhalte oder Konfiguration geändert wurden.

## Ergebnisse

- Zentraler Kursrahmen mit nachvollziehbaren Quellen, Annahmen und offenen Angaben
- Zeitlich stimmige Einheitenplanung mit messbaren Lernzielen und passenden Übungen/Verständnisprüfungen
- Konsistente README-Kursübersicht und erforderliche Modulbezüge in der Konfiguration
- Kurzer Bericht über Entscheidungen, tatsächliche Prüfungen und noch benötigte Kursinformationen

## Abschlussprüfung

- Sind offizielle Vorgaben und eigene Planungsentscheidungen klar getrennt?
- Passen Anspruch, Voraussetzungen, Prüfungsform und Lernzielprüfungen zusammen?
- Stimmen die Zeitsummen und bleiben eingeplante Übungen realistisch bearbeitbar?
- Sind bestehende Strukturen respektiert und größere Änderungsvorschläge begründet?
- Entspricht der dokumentierte Materialstatus tatsächlich vorhandenen Quellen und geprüften Ausgaben?

## Beispielaufträge

- „Planen Sie Digitaltechnik mit zehn Terminen à 90 Minuten auf Basis des bereitgestellten Modulhandbuchs.“
- „Überarbeiten Sie unseren Realzeit-Kursplan hinsichtlich der Lernzielabdeckung und erhalten Sie die bestehende Einheitenfolge.“
