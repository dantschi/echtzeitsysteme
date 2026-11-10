# Vorlesung 6: Abbildungen zu Server-Verfahren

Überarbeitung vom 10. November 2026, Ausgangsstand `e735dea`.
Grundlage sind sieben vom Lehrenden hochgeladene SVGs aus dem Vorjahr.
Ihre fachlich hilfreichen Darstellungsideen wurden als bearbeitbare Mermaid-
und PlantUML-Quellen in `vorlesungen/06_server-verfahren.qmd` neu ausgearbeitet.
Die Originaldateien wurden wegen der unten genannten Probleme nicht unverändert
eingebunden. Die Vorlesung behält ihre 50 Folien und die bisherigen Zahlenmodelle.

## Auswahl und Anpassungen

| Vorlage | Befund | Umsetzung in der aktuellen Vorlesung |
|---|---|---|
| `Grundprinzipder Serververfahren.svg` | Ereignis, Queue und Server sind anschaulich. Feste eingezeichnete Serverfenster gelten jedoch nicht allgemein; Angaben zu T1/T2 fehlen für einen überprüfbaren Ablauf. | Folie 8: Architektur mit Queue, Budget, Prioritätsauswahl und CPU; Bedingungspfeile von Datenfluss unterscheiden. |
| `PollingServer.svg` | Die Zeit von Ankunft 3 bis Bedienbeginn 25 ist Wartezeit, keine vollständige Antwortzeit. Eine allgemeine Abschlussgarantie „Worst Case ≤ Ts“ folgt daraus nicht. Die Bedienung bei leerer Queue ist ebenfalls missverständlich. | Folie 16: Im eingeführten Modell wartet A von 1 bis 8; Abschluss 9, Antwortzeit 8 ms. |
| `DeferrableServer.svg` | Ausführungsblöcke 24–29 und 26–31 überlappen auf einem Kern; die Auffüllgrenze 25 ist nicht sauber getrennt. | Folie 24: A läuft 6–8 aus altem, B 8–10 aus neuem Budget. Getrennte Spuren für Kern und Budgetperiode. |
| `ReplenishmentBuchfuehrung.svg` | Die Liste ist hilfreich. Generische Einträge ohne Verbrauchsgeschichte können eine pauschale periodische Rücksetzung nahelegen. | Folie 28: Abschnitte 1–2 und 6–7 erzeugen die Einträge (9,1) und (14,1); Buchung, Fälligkeit und verfügbares Budget ausdrücklich trennen. |
| `SporadicServer.svg` | SVG-Koordinaten enthalten nicht ausgewertete Ausdrücke, etwa `x="80+42"`. Die tatsächliche Darstellung entspricht dadurch nicht der beabsichtigten Zeitachse. | Folie 29: bestehende geprüfte SS-Achse um den Zustand von C ergänzt; C wartet trotz freiem Kern 10–14 auf Budget. Die späteren Rückgaben stehen in der folgenden Tabelle. |
| `SporadicServer2.svg` | Auch hier sind Koordinatenausdrücke ungültig, etwa `x1="80+67.2+210"`. | Folie 32: zweites vorhandenes SS-Modell als Zeitachse; der zusammenhängende Verbrauch von B erzeugt einen gebündelten Eintrag (17,2). |
| `SS_in_FreeRTOS.svg` | „RM/EDF-Äquivalent“ ist für die hier verwendete feste Prioritätsauswahl irreführend. Queue und Semaphor sind keine austauschbaren Auftragsspeicher; Timer und Zähler allein sind kein vollständiger SS. | Folie 38: Kernelbausteine von zusätzlicher CPU-Abrechnung, Rückgabeliste und Durchsetzung der Erschöpfung trennen. |

Alle sieben Darstellungen werden in den jeweiligen Lernnotizen erklärt.
Die Architektur- und Buchhaltungsgrafiken sind qualitative Mermaid-Diagramme;
die maßstäblichen Zeitachsen sind PlantUML-Timing-Diagramme mit Zeit in ms.
Die nativen Mermaid-SVGs erhalten zugängliche Titel und Beschreibungen.
Folien-CSS und eine auf VL6 begrenzte PDF-Grafikbegrenzung sichern Platz für
Beschriftungen und erläuternden Text. Der Glossarverweis auf Folie 8 wurde
an deren kürzere Überschrift angepasst.

## Fachliche Kontrolle

Die SS-Rechnungen bleiben ausdrücklich auf den eingeführten klassischen
Spezialfall beschränkt: keine höher priorisierte Arbeit, ununterbrochene aktive
Abschnitte und keine weitere Rückgabe innerhalb eines solchen Abschnitts.
Verbrauch wird bei Abschnittsende gebucht; fällig wird er bei Abschnittsbeginn
plus Serverzeitraum. Das ist keine allgemeine Regel für beliebige Präemptionen.

- Hauptmodell: Q=2, T=8; A(1,1), B(6,1), C(9,2).
  Polling-Abschlüsse 9/10/18, DS 2/7/11, SS 2/7/15 ms.
  SS-Rückgaben (9,1), (14,1), (17,1), (22,1).
- Zweites SS-Modell: Q=3, T=10; A(2,1), B(7,2), C(11,3).
  Abschlüsse 3/9/19; Rückgaben (12,1), (17,2), (22,1), (27,2).
- DS-Grenzfall: Q=2, T=8; A(6,2), B(8,2), sonst keine Arbeit.
  Vier CPU-Millisekunden zusammenhängend von 6 bis 10.

Eine unabhängige diskrete Modellrechnung und ein zweiter fachlicher Quellreview
bestätigen diese Werte und die SS-Annahmen. Die zwei engen C-Laufabschnitte
heißen in der Zustandsspur „CPU“; die Notizen erläutern die Prozessorzuteilung.
Die Darstellungen sind keine gemessenen FreeRTOS-Abläufe.

## Ausgabeprüfung

- Gezielter frischer Reveal.js- und Handout-Render mit der versionsgebundenen
  Projektumgebung. Zwischenprüfungen erkannten Mermaid-Syntaxfehler,
  zu enge Zeitbeschriftungen und eine im Handout unwirksame Größenanpassung;
  diese Fehler sind in der abschließenden Fassung behoben.
- Alle 50 HTML-Folien bei 1280 × 720 im Browser auf Überläufe geprüft;
  drei tatsächlich gerenderte Mermaid-SVGs, keine Fehlergrafik oder sichtbarer
  Diagrammquelltext. 50 Notizblöcke, im Vortrag nur in der Speaker View.
- Die sieben geänderten Diagrammfolien visuell geprüft. Für MathJax 2.7.9
  nutzte der Browserlauf das offizielle npm-Paket aus dem lokalen Testcache;
  die Auslieferungsadressen im Kurs bleiben unverändert.
- Handout: 36 Seiten, alle 50 Notiznummern vorhanden. Die PDF-Seiten
  5, 11, 16, 19, 20, 22 und 27 mit den sieben Grafiken wurden visuell geprüft.
  Deren Grafikbreite beträgt jeweils rund 432 pt. Eine zusätzliche Prüfung
  aller Textbegrenzungen fand keine Inhalte am Papierrand oder außerhalb
  der Seite. Das ersetzt keine Sichtprüfung aller 36 Seiten.
- UTF-8 ohne BOM, Folien-/Notizzuordnung, Zeitachsen und `git diff --check`
  geprüft. Die eigenständige fachliche Quellprüfung fand keine verbleibenden
  Inkonsistenzen in Modellwerten oder Erklärungen.
- Abschließender Gesamtbuild `python3 scripts/build-materials.py all` erfolgreich:
  Startseite und neun Vorträge, neun Handouts sowie zehn Aufgabenblätter in
  Studierenden- und Lösungsfassung. Im Kurs liegen keine Programmierpaket-Manifeste
  für zusätzliche ZIP-Prüfungen vor. Die sieben geprüften PDF-Seiten aus der
  endgültigen Ausgabe unter `_site/` stimmen pixelgenau mit dem gezielt
  gerenderten, sichtgeprüften Handout überein.

## Herkunft der Vorlagen

SHA-256 der geprüften Uploads, zur eindeutigen Zuordnung:

```text
c0e26db5ac897bcfadcd950690b12579673aedad203aaf5abd1541363d262815  Grundprinzipder Serververfahren.svg
7974ccb3692dcf914a12e35da47779021eb825aa28afb66355f248b5ed0a3a1f  PollingServer.svg
f61b33267f21f08846300f1b3721450655558074d2531af6cef09ae16680ebd0  DeferrableServer.svg
1cff5cc2d31a018397cf2fdf01be4f6b10f7d0bdb1c2319c4e85e39bddb84f6a  ReplenishmentBuchfuehrung.svg
901dcac72b17793c0a34710bbf48f415ddb6883fd844ef7f848806499ab1ba52  SporadicServer.svg
4a9fe46311be55ce5b596c58817edd8781a016863f6e4414645832222bffc216  SporadicServer2.svg
93408e9ab3954cc18e15bfac9149b78d3a85fa1a418e7983174e85b82fc19035  SS_in_FreeRTOS.svg
```
