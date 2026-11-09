# Übungsblatt 2: Suchen und Zählen

Sie ergänzen zwei Funktionen und prüfen deren Verhalten bei Treffern,
fehlenden Werten, Duplikaten und leeren Listen. Planen Sie für das
didaktische Beispiel 20–30 Minuten einschließlich Test und kurzer Begründung ein.
Vorausgesetzt werden Python-Funktionen, Listen, Vergleiche und Schleifen.

## Vorbereitung

Benötigt wird Python 3.12; weitere Pakete sind nicht erforderlich. Die folgenden
Befehle verwenden ein Terminal mit dem Befehl `python3`. Prüfen Sie zunächst:

```bash
python3 --version
```

Die Ausgabe muss mit `Python 3.12.` beginnen. Wenn Ihre Installation stattdessen
`python` oder `py -3.12` verwendet, ersetzen Sie `python3` in allen Aufrufen
entsprechend und prüfen Sie ebenfalls die Version.

Laden Sie `lab_02_suchen-startcode.zip` vom Übungsblatt bzw. der Kurswebsite
herunter und entpacken Sie das Archiv. Es enthält:

```text
lab_02_suchen/
  README.md
  starter/search.py
  tests/check.py
```

Wechseln Sie im Terminal in den entpackten Ordner und anschließend zu `starter`:

```bash
cd lab_02_suchen
cd starter
python3 -c "import search"
python3 ../tests/check.py
```

Der Import soll ohne Ausgabe und mit Exitcode 0 gelingen. Der erste Testlauf
meldet die vier noch offenen Testgruppen und endet absichtlich mit Exitcode 1:

```json
{"tests": 4, "failures": ["test_first", "test_notfound", "test_count", "test_empty"], "errors": []}
```

`failures` enthält IDs nicht erfüllter Testgruppen. Zu Beginn entstehen sie
durch die vorgesehenen `NotImplementedError`-Anweisungen; später können sie
falsche Ergebnisse anzeigen. `errors` enthält unerwartete Ausnahmen, etwa
einen Syntax- oder Importfehler. Solche Fehler gehören nicht zum erwarteten
Anfangszustand. Starten Sie den Test aus `starter/`, damit die richtige
`search.py` geladen wird. Meldet schon das Terminal, dass `python3` unbekannt
ist, prüfen Sie die Python-Installation.

## Aufgabe 1: Ersten Treffer finden

Bearbeiten Sie ausschließlich `starter/search.py`. Ergänzen Sie beim Kommentar
`TODO Aufgabe 1` die Funktion `first_index(values, target)`. Geben Sie den
nullbasierten Index des ersten Treffers zurück, bei fehlendem Wert `-1`.

| Aufruf | Ergebnis |
|--------|---------:|
| `first_index([4, 7, 7, 2], 7)` | `1` |
| `first_index([7, 2, 7], 7)` | `0` |
| `first_index([4, 2], 7)` | `-1` |
| `first_index([], 7)` | `-1` |

## Aufgabe 2: Alle Treffer zählen

Ergänzen Sie beim Kommentar `TODO Aufgabe 2` die Funktion
`count_matches(values, target)`. Geben Sie die Anzahl aller Treffer zurück;
bei fehlendem Wert oder leerer Liste ist das Ergebnis `0`.

| Aufruf | Ergebnis |
|--------|---------:|
| `count_matches([4, 7, 7, 2], 7)` | `2` |
| `count_matches([7, 7, 7], 7)` | `3` |
| `count_matches([4, 2], 7)` | `0` |
| `count_matches([], 7)` | `0` |

Für beide Aufgaben gelten dieselben Bedingungen: `values` ist eine endliche
Liste ganzer Zahlen, `target` eine ganze Zahl. Die Liste darf nicht verändert
werden. Behalten Sie Namen, Parameter und Rückgabetypen bei. Verwenden Sie
eine `for`- oder `while`-Schleife; `enumerate` ist erlaubt. Fertige Such- und
Zählfunktionen wie `list.index`, `list.count` und `sum` sind für diese Übung
nicht vorgesehen. Eingaben außerhalb dieses Vertrags werden nicht behandelt.

## Prüfen und abgeben

Führen Sie nach jeder Änderung aus `starter/` erneut aus:

```bash
python3 -c "import search"
python3 ../tests/check.py
```

Nach vollständiger Bearbeitung sollen alle vier Testgruppen bestehen und der
Test mit Exitcode 0 enden:

```json
{"tests": 4, "failures": [], "errors": []}
```

Die öffentlichen Tests prüfen ausgewählte Fälle; sie ersetzen keine
allgemeine Korrektheitsbegründung. Erklären Sie zusätzlich kurz, warum Ihre
Funktionen für jede zulässige Liste das geforderte Ergebnis liefern und
terminieren. Wie viele Gleichheitsvergleiche sind für eine Liste der Länge
$n$ im schlechtesten Fall nötig? Betrachten Sie dafür jeden Vergleich als
eine Operation, unabhängig von der Größe der ganzen Zahlen; reale Laufzeiten
werden hier nicht gemessen.

Als Arbeitsergebnis benötigen Sie die ausgefüllte `starter/search.py` und Ihre
kurze Begründung gemäß den Abgabevorgaben Ihrer Lehrveranstaltung. Ändern Sie
die mitgelieferten Tests nicht. Das Startcode-Archiv enthält keine Musterlösung.
