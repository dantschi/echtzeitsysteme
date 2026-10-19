# Projektvorbereitung zu Übungsblatt 1

Verwenden Sie STM32CubeMX, VS Code mit STM32 VS Code Extension und **NUCLEO-H755ZI-Q / STM32H755ZIT6**. Die installierte Kombination muss die gewählten CM7-/CM4-CMake-Ziele unterstützen. Erhalten Sie die generierte Dual-Core-Boot-/HSEM-Startfolge und laden Sie zusammengehörige Images. C11 oder neuer; fremde Laborlast und doppelte Callbacks/Threadanlagen entfernen. Eigene Quellen und Benutzerbereiche müssen eine erneute CubeMX-Generierung mit anschließendem Build überstehen.

Die Programmieraufgabe wird von den Studierenden selbst implementiert. Bereiten Sie Installation, Boot, GPIO und gegebenenfalls Kernel, Linker und MPU vor dem Termin vor. Alle Versuche verwenden ausschließlich eingebaute LEDs, B1 und ST-LINK. Kein Sensor, Steckbrett, externer Widerstand, UART-Aufbau, Oszilloskop oder zusätzlicher Traceadapter ist erforderlich. Die Papieraufgaben sind unabhängig vom Aufbau lösbar; die Lehrperson kann aus dem Aufgabenpool auswählen.

## Tasterbasis auf CM4

1. CM4 ohne RTOS, laufende HAL-Zeitbasis mit 1 kHz. B1 ist im vorgesehenen MB1363-Boardplan an PC13, aktiv HIGH; prüfen Sie Revision und tatsächlichen Ruhepegel. Konfigurieren Sie als Eingang `LAB01_BUTTON`, keinen EXTI-IRQ. Die vorhandene Boardbeschaltung verwenden; keine externe Entprellschaltung ergänzen.
2. LD1/PB0 als `LAB01_LED`, LD2/PE1 als `LAB01_STATUS`, Ausgänge anfangs LOW, ausschließlich CM4 zugeordnet. Prüfen Sie Boardbelegung/Polarität gegen UM2408. CM7 darf diese Pins und ihre Portkonfiguration nicht verändern.
3. Nur `lab01_button.c`, `button_step.c` und deren Headerpfad in das CM4-Ziel aufnehmen. `lab01_demo.c`, seine Timer-/LED-Aufrufe und früherer Anwendungscode werden für diesen Versuch nicht gebaut; die funktionierende HAL-Zeitbasis bleibt erhalten.
4. Im erhaltenen Include-Bereich `lab01_button.h` einbinden. Nach Boot/HAL/GPIO einmal `Lab01_ButtonInit();`, innerhalb der vorhandenen Hauptschleife `Lab01_ButtonRunOnce();`. Keine weitere Delay-Schleife hinzufügen.
5. Vorbereitungsprüfung: unveränderte Startdatei lässt LD1 aus und LD2 ungefähr alle 250 Tickwerte umschalten; CM4 erreicht `Lab01_ButtonStep`. Die vorbereitete 20-ms-Stabilitätsfilterung wird durch wiederholte Aufrufe bedient. Studierende ersetzen anschließend den leeren Rumpf und testen die Nachleuchtanzeige. Auch bei beim Start gehaltenem B1 zählt die erste Betätigung gemäß Aufgabenvertrag.
6. Für einen Überlauftest zunächst eine private Rechnung oder Hostsimulation verwenden, nicht unkoordiniert die Systemzeit verstellen. Zeitvergleiche setzen einen laufenden Tick und keine unbeobachteten vollständigen Umläufe voraus.

Der frühere Timer-/Lastvergleich bleibt separat in `lab01_demo.c/.h` und im Code-README verfügbar; er wird nicht mit dieser Anwendung kombiniert.

## Prüfstatus und Abgrenzung

Geben Sie einen vor dem Termin auf der tatsächlichen Hardware gebauten und gestarteten Basisstand aus. Die studentische Startdatei enthält bewusst noch keine Aufgabenlösung. Aufgaben und vollständige Musterfunktionen stehen im zugehörigen Übungsblatt, Lösungen nur im Quarto-Profil `solution`. Ein Source-/PDF-Build ist kein erfolgreicher Firmware- oder Boardtest. Die Cloudprüfung kann Anwendungslogik am Host prüfen, jedoch keine STM32-Takte, GPIO-/IRQ-Zuordnung, Speicherattribute oder reale Zeitgrenzen bestätigen. Ungemessene Zeiten bleiben offen.
