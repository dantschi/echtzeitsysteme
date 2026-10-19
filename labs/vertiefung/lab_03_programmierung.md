# Projektvorbereitung zu Übungsblatt 3

Aufgabe 6 lässt die Studierenden das Vorlesungsbeispiel **HeavyMath, Heartbeat und Tasterreaktion** selbst implementieren. **Ausschließlich CM4 läuft**: FreeRTOS, Anwendung, Systemstart und Taktkonfiguration liegen auf M4. Verwendet werden NUCLEO-H755ZI-Q / STM32H755ZIT6, STM32CubeMX und VS Code mit STM32 VS Code Extension; die eingebauten LEDs LD1/LD2, B1 und USB/ST-LINK genügen.

## CM4 ohne laufenden M7 vorbereiten

1. Erstellen Sie vor dem Termin eine für **STM32H755ZIT6 / Cortex-M4** konfigurierte CMake-Projektbasis mit passendem Startup, Vektortabelle und Linkerscript. Ein von CubeMX zusätzlich angelegtes CM7-Ziel wird hier nicht gebaut, geladen oder gestartet. Die übliche Dual-Core-Bootfolge darf nicht unverändert übernommen werden: CM4 darf weder per HSEM auf eine Freigabe von CM7 warten noch deshalb in D2-STOP/WFE gehen. Auch Debugger und Startskripte dürfen CM7 nicht automatisch starten.
2. Stellen Sie die Boot-Option-Bytes mit STM32CubeProgrammer einmal für diese Projektbasis auf **BOOT_CM4 / BCM4 = 1** und **BOOT_CM7 / BCM7 = 0**. Prüfen Sie, dass die aktive CM4-Bootadresse (`BOOT4_ADD0` bei der gewählten Bootkonfiguration), Flash-Linkadresse und Vektortabelle zusammenpassen. Nur das Weglassen des CM7-Images bei unverändertem Dual-Core-Start reicht nicht. Bootkonfiguration und verwendete Werkzeugversionen gehören zur ausgegebenen Projektbasis.
3. CM4 übernimmt `HAL_Init()`, die zur Boardversorgung passende Spannungs-/Taktinitialisierung und danach GPIO, HAL-Zeitbasis und RTOS-Initialisierung. Als ST-Referenz dient [STM32CubeH7, BootCM4_CM7Gated](https://github.com/STMicroelectronics/STM32CubeH7/tree/8ec3113d4039d30ba9dc2bf0cb6eefc756b48d06/Projects/NUCLEO-H745ZI-Q/Templates/BootCM4_CM7Gated). Dort initialisiert CM4 bereits Systemtakt und Spannung, gibt anschließend jedoch CM7 frei. **Diese Freigabe `HAL_RCCEx_EnableBootCore(RCC_BOOT_C1)` entfällt hier vollständig.** Die Referenz ist auf STM32H745 ausgerichtet; MCU-, Versorgungs- und Taktparameter müssen zum H755-Projekt passen. ST nennt die NUCLEO-H745ZI-Q-Projekte im [H755-Kompatibilitätshinweis](https://github.com/STMicroelectronics/STM32CubeH7/blob/8ec3113d4039d30ba9dc2bf0cb6eefc756b48d06/Projects/NUCLEO-H755ZI-Q/readme.txt) als kompatibel. Das ersetzt die Boardprüfung nicht.
4. Prüfen Sie einen Kaltstart nach aus- und wieder eingeschalteter Versorgung, auch ohne Debugger: CM4 erreicht `main()` und den Kernel, während CM7 gesperrt bleibt. Ein nur durch den Debugger gesetzter Programmeinstieg ist kein Nachweis der Bootkonfiguration. Für diese Aufgabe sind weder ein CM7-Bootloader noch ein zweites Image nötig.

## GPIO, Kernel und Einbindung

1. Konfigurieren Sie **FreeRTOS ausschließlich auf CM4**, **CMSIS_V2**, Präemption, 32-Bit-Kernelticks und **1000 Hz**. Prüfen Sie die erzeugte `FreeRTOSConfig.h`, die CMSIS-Prioritätsabbildung und die Cortex-M4-Portauswahl. SysTick gehört dem Kernel; für die HAL-Zeitbasis wird in dieser Projektvariante ein freier **TIM6** verwendet. Prüfen Sie beide Handlerketten und die aus der tatsächlichen CM4-Taktkonfiguration abgeleiteten Zeitbasen.
2. Konfigurieren Sie **LD2/PE1** und **LD1/PB0** als CM4-Ausgänge, anfangs LOW, sowie **B1/PC13** als EXTI-Eingang mit der Flanke beim Drücken. Pinbelegung, aktive Polarität und Boardrevision sind gegen UM2408 zu prüfen. Nur der Heartbeat schreibt PE1, nur der Ereignis-Thread PB0. Keine weitere Quelle verwendet in dieser Basis `EXTI15_10_IRQn`.
3. Konfigurieren Sie EXTI15_10 mit einer für nichtblockierende FreeRTOS-Aufrufe zulässigen logischen NVIC-Priorität. Beispiel: Bei vier implementierten Prioritätsbits, passender Gruppierung und `configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY = 5` ist der unverschobene Wert **5** zulässig, 0 bis 4 sind es nicht. Entscheidend ist die konkrete `FreeRTOSConfig.h`; die numerischen CMSIS-Threadprioritäten sind eine andere Skala. **Die NVIC-Freigabe bleibt während der gesamten Initialisierung aus.** Entfernen Sie ein automatisch erzeugtes vorzeitiges Enable aus der wirksamen Startfolge, auch nach erneuter Generierung.
4. Erhalten Sie im CM4-IRQ-Handler die Kette zu `HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13)`. Ergänzen Sie genau eine Weiterleitung im bestehenden Callback; erzeugen Sie keine zweite Definition. Include in einem erhaltenen Benutzerbereich:

```c
#include "lab03_rtos.h"
```

   Callback-Inhalt für die vorbereitete Basis:

```c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    Lab03_ButtonIRQ(GPIO_Pin);
}
```

   Die Hilfe ignoriert andere Pins, prüft Kernelzustand und Handle, setzt `LAB03_BUTTON_FLAG` und zählt API-Fehler in `lab03_irq_errors`. Sie wartet nicht und schaltet keine LED. `Lab03_EnableButtonIRQ()` verwirft später im laufenden Ereignis-Thread alte PC13-/NVIC-Anforderungen und gibt den IRQ frei. Danach wird kein Thread-Flag blind gelöscht. Flanken vor dieser Freigabe werden absichtlich verworfen.
5. Nehmen Sie **`lab03_rtos.c`**, **`threads_student.c`** und den Headerpfad aus `code/lab03/` in das **CM4-CMake-Ziel** auf; C11 oder neuer verwenden. Der Header weist CM7-Builds zurück. Entfernen Sie unbenutzte generierte Default-Threads und deren Anlage. Keine zusätzlichen Kopien der drei Threads oder frühere Testlast dürfen mitlaufen. Die früheren Füllstandsdateien `lab03_tank.c/.h` und `tank_threads.c` gehören nicht zu dieser Variante.
6. Rufen Sie nach dem einen vorhandenen `osKernelInitialize()`, im Zustand `osKernelReady`, und **vor** dem einen vorhandenen `osKernelStart()` genau einmal auf:

```c
Lab03_Start();
```

   Bewahren Sie Include und Aufruf in erhaltenen Benutzerbereichen bzw. einer von dort aufgerufenen eigenen Initialisierung auf. Doppeln Sie Kernelinitialisierung und -start nicht. Eine unerwartete Rückkehr aus `osKernelStart()` gehört in den Fehlerpfad. Das Modul prüft Tickfrequenz und alle drei `osThreadNew`-Ergebnisse vor dem Kernelstart.
7. Die vorgegebenen CMSIS-Attribute sind Event **High / 1024 Byte**, Heartbeat **Normal / 512 Byte** und HeavyMath **BelowNormal / 1024 Byte**. Diese Stackgrößen sind Ausgangswerte, keine nachgewiesenen Minima. Heap/RAM müssen zusätzlich Idle, gegebenenfalls Timer-Task, Verwaltungsdaten und Handlerstack berücksichtigen. Die Handles werden vor dem Kernelstart angelegt; das intern gespeicherte Ereignishandle bleibt danach unverändert.
8. Bauen und laden Sie nur das CM4-Image. In den unveränderten studentischen Rümpfen warten alle drei Threads: beide LEDs bleiben aus, der Taster-IRQ bleibt gesperrt. Prüfen Sie, dass die drei Eintrittsfunktionen erreicht werden und anschließend blockieren. Dieser Stand prüft die Basis, noch nicht die Anwendung. Prüfen Sie erneute CubeMX-Generierung, Build und Kaltstart auf den Erhalt **aller** CM4-Anpassungen. Geben Sie diesen Stand vor dem Termin aus.

## Bearbeitung und Auswertung

Die Studierenden ersetzen die drei Funktionsrümpfe in `threads_student.c` und entfernen danach `WaitForImplementation`. Sie implementieren Heartbeat-Schleife, dauerhafte Rechenschleife und blockierenden Flagempfang. Die vollständigen Musterfunktionen stehen ausschließlich im `solution`-Profil des Übungsblatts; sie werden bei Bedarf anstelle der studentischen Definitionen gebaut. Threadanlage und sichere IRQ-Freigabe sind bereits vorbereitet.

`Lab03_WaitHeartbeat` erhöht das vorherige absolute Sollziel um 500 Ticks. Bei verpasstem Ziel zählt es eine Zielverfehlung und richtet auf jetzt plus 500 neu aus. Läuft das Ziel im API-Übergang ab, wird einmal mit einem neuen Ziel versucht; weitere Fehler führen zu `Lab03_Fail`. Eine sofortige Nachholserie wird vermieden. Die Moduloauswertung setzt unbeobachtete Zeitabstände unter der halben 32-Bit-Spanne voraus. Der Zähler ist ohne weiteren Zeitvertrag kein allgemeiner Deadline-Miss-Zähler und kann insbesondere keine Fehler erfassen, während der Heartbeat gar nicht läuft.

Für den Starvation-Versuch wird nur `.priority` in `math_attr` innerhalb `Lab03_Start()` auf `osPriorityAboveNormal` geändert. Nach Build, Flash und Neustart verhungert der Heartbeat, während Event weiterhin über HeavyMath liegt. Anschließend die ursprüngliche Priorität wiederherstellen. Ein Yield macht HeavyMath nicht Blocked und gibt dem niedrigeren Heartbeat keine verlässliche Ausführungsmöglichkeit.

Die Diagnosefelder haben jeweils nur einen schreibenden Kontext und werden gemeinsam nur bei angehaltenem CM4 gelesen. `volatile` macht Zugriffe beobachtbar und ersetzt keine Synchronisation. Tasterprellen und das Zusammenfallen mehrerer Flag-Setzungen verhindern eine garantierte Eins-zu-eins-Zählung von Tastendrücken. Die Aufgabe verlangt keinen Entprellalgorithmus und keinen numerischen Reaktionszeitnachweis.

Die Dateien sind Anwendungsbausteine und studentische Startdateien, kein generiertes Firmwaregesamtprojekt. Ein Hosttest ersetzt weder den ARM-Build noch den tatsächlichen Kaltstart und Boardlauf. Ohne funktionierende Hardware bleiben die Papieraufgaben lösbar; ein nicht ausgeführter Boardtest wird benannt.
