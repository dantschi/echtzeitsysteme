---
name: Review-Verbesserungsplan EZS
overview: Konsolidierter Verbesserungsplan aus dem kritischen Gesamt-Review aller 9 Vorlesungen, 9 Übungen und der Probeklausur — gegliedert in fachliche Korrekturen, Konsistenz, Layout/Rendering, Didaktik/Struktur und Repo-Hygiene.
todos:
  - id: fach-ewi-hsem-led
    content: "Fachfehler-Kette korrigieren: EWI!=NMI (VL8, Lab8, Probeklausur), HAL_HSEM_ReleaseAll statt ClearAll, LED PE1=gelb/PB14=rot"
    status: pending
  - id: fach-vl1-3
    content: "VL1-3: RMS-Timeline (C3), Labor-Framing Fixed-Priority statt RMS, Latenz vs. Response Time, API-Konsistenz osDelay"
    status: pending
  - id: fach-schaerfungen
    content: Kleinere fachliche Schärfungen einarbeiten (Liu-Layland, PendSV, Write-Back, MC/DC, ORTI, WWDG 0x40, 1oo2, PCLK1)
    status: pending
  - id: konsistenz
    content: "Konsistenz: sd/shared_data, Taktfrequenzen VL6, IPC->Inter-Task, Titel-Nomenklatur, Lab-5-Queue-Lücke"
    status: pending
  - id: layout-mu
    content: µs-Rendering-Bug im Handout-PDF beheben und alle Handouts neu rendern/prüfen
    status: pending
  - id: layout-diagramme
    content: Diagramm- und Folien-Hotspots fixen (VL8 WaveDrom, VL6 Cache-Timing, VL4 PIP-Umbruch, VL3 Zustandsdiagramm->PlantUML, VL2/VL1 Mermaid-Höhe)
    status: pending
  - id: layout-labs
    content: "Lab-PDFs: Code-Seitenumbrüche entschärfen, Lab-3-Glitch 'W arumosDelay' fixen"
    status: pending
  - id: didaktik-vl9
    content: "VL9 vervollständigen: Pflichtenheft-Folien, Labor-Checkliste, Abschlussfolie; Redundanzen VL5/VL8 straffen"
    status: pending
  - id: repo-hygiene
    content: "Repo-Hygiene: _tmp_*-Dateien löschen, Lab-9-PDFs enttracken, build_labs.sh fixen, Altartefakte und .gitignore-Encoding bereinigen"
    status: pending
  - id: verify
    content: Alles lokal rendern, PDFs stichprobenartig prüfen, committen, pushen, CI-Lauf verifizieren
    status: pending
isProject: false
---

# Verbesserungsplan Echtzeitsysteme (EZS) — Gesamtreview

Basis: vier parallele Detail-Reviews ([VL1–3](7a09c561-79d2-44b2-ba64-5a34f71944d7), [VL4–6](6f86d094-d65c-4708-801d-4b6880ee8035), [VL7–9](e7b9c59e-0b28-4334-97ac-0553fe09ec3b), [Übungen/Probeklausur](cf37df14-9ae7-41eb-9aba-f28abecfc840)) plus eigene Prüfung von Build-Pipeline und Repo. Gesamteindruck: fachlich tragfähig, Notes flächendeckend vorhanden, roter Faden stimmig. Die kritischen Punkte sind einzelne harte Fachfehler, Konsistenzbrüche zwischen Vorlesung und Labor sowie Rendering-Probleme in den Handout-PDFs.

## Phase 1: Fachliche Fehler (hohe Priorität)

- **EWI ist kein NMI** — Fehlerkette durch drei Dateien: Der WWDG-Early-Wakeup-Interrupt ist ein maskierbarer NVIC-Interrupt (`WWDG_IRQn`), kein ARM-NMI. Korrigieren in [vorlesungen/08_multicore-synchronisation-safety.qmd](vorlesungen/08_multicore-synchronisation-safety.qmd) (~Z. 567), [labs/lab_08_hsem-wwdg-safe-shutdown.qmd](labs/lab_08_hsem-wwdg-safe-shutdown.qmd) und [labs/probeklausur.qmd](labs/probeklausur.qmd) (Erwartungshorizont A4.2).
- **`HAL_HSEM_ClearAll` existiert nicht** — VL8-Folie „Clear by Core ID“ (~Z. 292): korrekt ist `HAL_HSEM_ReleaseAll(Key, CoreID)`. Code kompiliert sonst nicht.
- **LED-Zuordnung PE1** — VL8 (~Z. 807) und Lab 8 Aufgabe 4 nennen PE1 „rote Fehler-LED“; auf dem Nucleo-H755ZI-Q ist PE1 die gelbe LD2, rot ist PB14 (LD3). Einheitlich korrigieren (VL1/Lab1 nutzen PE1 korrekt als LD2).
- **VL2 RMS-Timeline widersprüchlich** — [vorlesungen/02_scheduling-theorie.qmd](vorlesungen/02_scheduling-theorie.qmd) (~Z. 559–625): Task-Set hat C3=1, Timeline/Text zeigen aber Präemption von Tau3 über 2 Zeiteinheiten. C3=2 setzen und Timeline/Notes/RTA-Beispiel angleichen.
- **VL3-Labor heißt fälschlich „RMS live beweisen“** — Prioritäten sind nach Kritikalität vergeben, nicht nach Periode; das demonstriert Fixed-Priority-Präemption. Umformulieren in [vorlesungen/03_einfuehrung-rtos.qmd](vorlesungen/03_einfuehrung-rtos.qmd) (~Z. 736, 869); ebenso `configUSE_PREEMPTION`-Folie („RMS-Stil“ → „präemptiv/Fixed Priority“).
- **VL1 Latenz vs. Response Time** — Folientitel setzt Latenz mit Response Time gleich; sauber abgrenzen (Interrupt-Latenz hier, R_i formal erst in VL2).
- **VL4 Coffman-Bedingungen ergänzen + Starvation/Inversion auf die Folie** — Deadlock nur anekdotisch behandelt; eine Folie mit den vier Bedingungen ergänzen. Abgrenzung Starvation vs. Priority Inversion steht nur in den Notes, gehört auf die Folie. Zudem Notes VL8 (~Z. 258): Busy-Wait-Polling ist keine „Prioritätsinversion“.
- **`os_yield`-Rekursionsproblem kennzeichnen** — In VL2 (~Z. 961) und Lab 2 ruft `os_yield()` die Task-Funktion rekursiv auf → Stack-Wachstum. Als bewusste didaktische Vereinfachung markieren (mit Warnhinweis) oder dispatch-basiert umbauen.
- **Kleinere fachliche Schärfungen** (mittel/niedrig, gesammelt): Liu-Layland als hinreichende, n-abhängige Schranke formulieren (VL2-Tabelle „69,3 %“); PendSV-Schrittfolge an FreeRTOS-Port angleichen; „Write-Back = Standard“ relativieren (VL6); MC/DC „highly recommended“ statt „Pflicht“ für ASIL D, ORTI vs. FreeRTOS-Plugin trennen, empirische WCET als beobachtete Schranke (VL7); WWDG-Reset bei 0x40→0x3F, „AXI-Arbiter“ → „Bus-Matrix-Arbiter“ (VL8); 1oo2-Definition schärfen (VL9); WWDG-PCLK1-Annahme in Lab 8 explizit machen.

## Phase 2: Konsistenz Vorlesung ↔ Labor ↔ Klausur

- **Shared-Memory-Variablenname vereinheitlichen**: VL6 Theorie nutzt `shared_data`, Laborteil und Lab 6 `sd`, VL7 teils `ping_pong_data`. Kanonisch auf `sd` (wie Labs 6–9) umstellen in [vorlesungen/06_multicore-echtzeit.qmd](vorlesungen/06_multicore-echtzeit.qmd) und [vorlesungen/07_tracing-timing-analyse.qmd](vorlesungen/07_tracing-timing-analyse.qmd) (~Z. 260, 600); Linker-Name durchgängig `RAM_D3`.
- **Taktfrequenz-Narrativ kanonisieren** (VL6): Specs 480/240 MHz vs. RCC-Folie 400/200 MHz vs. Cache-Notes „400 MHz“. Eine Folie „Max-Spec vs. Board-Setup (400/200)“, alle anderen Stellen darauf beziehen (VL7 rechnet mit 400 MHz — passt dann).
- **API-Konsistenz FreeRTOS vs. CMSIS** (VL2-Ausblick verspricht `vTaskDelay`, VL3/Labs nutzen `osDelay`): Ausblick anpassen; in VL3 ab der CMSIS-Folie nur noch CMSIS-API, native API als kompakter Vergleich; Lab-2-Scheduler-API einheitlich (`os_yield`/`os_delay`).
- **Terminologie „IPC“ → Inter-Task-Kommunikation** in VL5 (Agenda/Folien) und VL3 (~Z. 96), einmal explizit definieren; VL3 „Port = HAL“ → „Port-Layer“.
- **Titel-Nomenklatur „&“ vs. „und“** über alle Übungsblätter und [README.md](README.md) vereinheitlichen (Empfehlung: „und“ in Titeln, „&“ nur in engen Tabellen).
- **Lab 5 Themenlücke Queues**: VL5/README betonen Queues, das Blatt übt nur Semaphor-DIP. Entweder Mini-Queue-Aufgabe ergänzen (empfohlen, schließt Lücke zur Probeklausur/Capstone) oder Titel/README auf „Deferred Interrupt Processing“ eingrenzen.

## Phase 3: Layout und PDF-Rendering

- **Mikrosekunden-Bug im Handout-PDF (VL7, systematisch)**: `\mathrm{\mu s}` rendert als „s“ — Studierende lesen „430 Sekunden“. Ursache im LaTeX-Font-Stack (TeX Gyre/unicode-math) beheben, z. B. `\text{µs}`/`\textmu` oder `\,\mu\mathrm{s}`-Schreibweise testen; danach alle Handouts stichprobenartig auf µ prüfen (betrifft auch Context-Switch „2,5 s“).
- **Diagramm-Hotspots**:
  - VL8 WaveDrom WWDG-Fenster (PDF S. 18): überlappende Labels, abgeschnittener Hinweis → WaveJSON vereinfachen/Folie splitten; Labor-Code-Folie S. 27 mit abgeschnittenen Kommentaren kürzen.
  - VL6 PlantUML Cache-Coherency-Timing (S. 19): Label-Überlappung → kürzere Labels; SMP/AMP-Architektur-Diagramme regelkonform auf Mermaid umstellen, PlantUML aus Columns nehmen.
  - VL4: PIP-Folie wird im Handout über Seitenumbruch zerrissen (S. 22–23); Mermaid-Deadlock-Graph mit gedrängten Labels.
  - VL3: FreeRTOS-Zustandsdiagramm als Mermaid `stateDiagram-v2` verstößt gegen `dhbw-diagramme.mdc` (RTOS-Zustände = PlantUML), Label „Dispatch (Schedul“ abgeschnitten → auf PlantUML umbauen.
  - VL2: OS-Architektur-Mermaid extrem hoch (~900×3460) → kompakt als `LR`; VL1: Polling-Mermaid zu hoch, Jitter-WaveDrom zu klein, Priority-Inversion-Teaser-Diagramm entschlacken (voll erst in VL4).
- **Handout-Dichte VL6**: Notes sind teils Vortragsessays und blähen das Handout auf → auf Didaktik-Hinweise kürzen, lange Codeblöcke auf eigene Folien.
- **Musterlösungs-PDFs der Labs**: Seitenumbrüche mitten im Code (Labs 1, 2, 4, 6, 8, 9) → Codeblöcke kürzen/teilen oder gezielt `\pagebreak`; Render-Glitch „W arumosDelay“ in Lab 3 fixen; VL8-Handout „→Besetzt“ ohne Leerzeichen.

## Phase 4: Didaktik und Struktur

- **VL9 vervollständigen**: Agenda verspricht ein Capstone-Pflichtenheft, die Vorlesung endet aber abrupt nach dem Architekturdiagramm. Ergänzen: 1–2 Folien Muss-/Abnahmekriterien (EXTI→Queue, HSEM, WWDG-Fenster, M7-Filter/UART, BlueBox-Latenzmessung) plus Labor-Checkliste und Abschlussfolie analog VL7/8 — Inhalte aus [labs/lab_09_capstone-timing-verifikation.qmd](labs/lab_09_capstone-timing-verifikation.qmd) verdichten.
- **Redundanzen markieren/straffen**: VL5-Recap des Deferred-Interrupt-Musters explizit als „Vertiefung von VL4“ kennzeichnen; VL8-Folie „Warum WWDG?“ (wiederholt Fenster-Konzept) streichen oder in Notes; Zustandsmodell VL2 kurz, VL3 vertieft.
- **Laborumfang managen**: Lab 6 (Dual-Core+Linker+MPU) und Lab 9 (Capstone) sind sehr voll → „Minimum-Viable“-Checkliste bzw. Teilabnahmen und Zeitboxen ergänzen; Lab 7 Voraussetzung (Lab-6-Projekt mit MPU absichtlich aus) explizit machen.
- **HSEM-Polling im Labor** (VL8/Lab 8) als pragmatische Stufe gegenüber der Interrupt-Variante der Theorie kennzeichnen.

## Phase 5: Repo-Hygiene und Build-Pipeline

- Neun `_tmp_*`-Dateien im Root löschen (untracked Arbeitsreste).
- `labs/lab_09_capstone-timing-verifikation.pdf` und `-musterloesung.pdf` enttracken (`git rm --cached`) — PDFs sind laut `.gitignore` Build-Artefakte; Inhalt geprüft, kein Lösungs-Leak.
- [build_labs.sh](build_labs.sh) fixen: Nach dem letzten Render bleiben `labs/*.pdf` lokal als Musterlösung liegen (deshalb sind `lab_01_….pdf`/`lab_03_….pdf` lokal Lösungen). Reihenfolge tauschen (Solution zuerst, Student zuletzt) oder am Ende Studierenden-Version zurückkopieren. Wichtig: Die veröffentlichte Website ist nicht betroffen — `publish.yml` kopiert korrekt getrennt.
- Lokale Altartefakte `labs/lab_01_student_check.pdf` und `lab_03_…-studierende.pdf` löschen; zugehörigen Eintrag in `labs/.gitignore` entfernen.
- Kaputte Umlaute in den Kommentaren der Root-[.gitignore](.gitignore) reparieren (UTF-8).

## Umsetzung und Verifikation

Reihenfolge: Phase 1+2 zuerst (ein Commit pro Vorlesung/Lab-Paar), dann Phase 3 mit lokalem Neu-Rendern (Handout-Profil + beide Lab-Profile) und seitenweiser PDF-Stichprobe (insbesondere µs-Darstellung), dann Phase 4, zuletzt Phase 5. Abschließend Commit/Push und CI-Lauf (`publish.yml`) prüfen.