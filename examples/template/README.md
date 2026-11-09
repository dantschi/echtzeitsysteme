# Getrennte Beispiele aus dem Quarto-Template

Diese Beispiele zeigen die Konventionen des zugrunde liegenden
[DHBW-Quarto-Templates](https://github.com/dantschi/dhbw-quarto-template).
Sie gehören nicht zu den neun Vorlesungseinheiten oder Übungsblättern des
EZS-Kurses. Der reguläre Materialbuild verarbeitet dieses Verzeichnis nicht.

- [Vorlesungsbeispiel mit aktivierender Einstiegsfrage](vorlesungen/01_kickoff.qmd)
- [Suchaufgabe mit Startcode und Musterlösung](labs/lab_02_suchen.qmd)
- [Programmierprojekt mit Einrichtung und Testvertrag](code/labs/lab_02_suchen/README.md)

Die Suchaufgabe verwendet Python 3.12 und benötigt keine weiteren Pakete.
Die eigentlichen EZS-Programmieraufgaben verwenden weiterhin die in ihren
Blättern angegebenen STM32-Werkzeuge und Boards.

## Startcode, Referenzlösung und ZIP prüfen

Führen Sie den folgenden Aufruf aus dem **Repository-Stamm** aus. Der gemeinsame
Paketierer prüft den dokumentierten unvollständigen Startzustand, die vollständige
Referenzlösung und anschließend das tatsächlich entpackte Studierendenpaket.
Der Startcode bleibt dabei absichtlich unvollständig; die Prüfungen laufen in
temporären Kopien.

```bash
python3 - <<'PY'
from pathlib import Path
import shutil
import sys

repo = Path.cwd()
sys.path.insert(0, str(repo / "scripts"))
from programming_labs import build_programming_packages

example = repo / "examples" / "template"
stage = repo / ".quarto" / "template-example-packages"
artifacts = build_programming_packages(example, stage)
for relative, archive in artifacts.items():
    destination = example / relative
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(archive, destination)
    print(destination.relative_to(repo))
PY
```

Erwartetes Paket:
`examples/template/_site/labs/lab_02_suchen-startcode.zip`.
Es enthält nur die freigegebene Anleitung, `starter/search.py` und
`tests/check.py`, keine Referenzlösung. Das Verzeichnis `_site/` des Beispiels
ist ignoriert und getrennt von der Veröffentlichung des EZS-Kurses.

Für eigene Kursaufgaben gelten die [Paketregeln](../../code/labs/AGENTS.md).
Übernehmen Sie das passende Blatt nach `labs/` und das gleichnamige Projekt
nach `code/labs/` erst, wenn es tatsächlich Teil des Kurses werden soll.
Der normale Kursbuild erzeugt dann die geprüften Startcode-Pakete zusammen
mit den Materialien; seine frischen PDFs liegen in `_site/labs/` und
`labs/_output/`, nicht im Quellverzeichnis `labs/`.
