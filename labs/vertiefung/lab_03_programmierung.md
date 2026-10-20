# Projektvorbereitung zu Übungsblatt 3

Aufgabe 6 verwendet **zwei in CubeMX angelegte Anwendungsthreads auf M4**: Heartbeat schaltet LD2/PE1 und wartet mit `osDelay(500)`; HeavyMath rechnet und schaltet nach jedem Block LD1/PB0. Die Studierenden schreiben direkt in die generierten Threadfunktionen. Die benötigten Anwendungsbausteine, konkreten CubeMX-Einstellungen und Rechenschleifen stehen im [Übungsblatt, Aufgabe 6](../lab_03_einfuehrung-freertos-praeemption.qmd#aufgabe-6-zwei-freertos-threads-mit-leds). Zusätzliche Hilfsdateien aus dem Repository werden nicht eingebunden.

Diese Anleitung ergänzt die Aufgabe um den **vor dem Termin zu prüfenden eigenständigen M4-Start und die gemeinsame SysTick-Zeitbasis**. Verwendet werden NUCLEO-H755ZI-Q / STM32H755ZIT6, STM32CubeMX, VS Code mit STM32 VS Code Extension und USB/ST-LINK. CM7 bleibt deaktiviert. Taster, EXTI und zusätzliche Peripherietimer sind nicht erforderlich.

## CM4 ohne laufenden M7 vorbereiten

1. Erstellen Sie vor dem Termin eine für **STM32H755ZIT6 / Cortex-M4** konfigurierte CMake-Projektbasis mit passendem Startup, Vektortabelle und Linkerscript. Ein von CubeMX zusätzlich angelegtes CM7-Ziel wird hier nicht gebaut, geladen oder gestartet. Die übliche Dual-Core-Bootfolge darf nicht unverändert übernommen werden: CM4 darf weder per HSEM auf eine Freigabe von CM7 warten noch deshalb in D2-STOP/WFE gehen. Auch Debugger und Startskripte dürfen CM7 nicht automatisch starten.
2. Stellen Sie die Boot-Option-Bytes mit STM32CubeProgrammer einmal für diese Projektbasis auf **BOOT_CM4 / BCM4 = 1** und **BOOT_CM7 / BCM7 = 0**. Prüfen Sie, dass die aktive CM4-Bootadresse (`BOOT4_ADD0` bei der gewählten Bootkonfiguration), Flash-Linkadresse und Vektortabelle zusammenpassen. Nur das Weglassen des CM7-Images bei unverändertem Dual-Core-Start reicht nicht. Bootkonfiguration und verwendete Werkzeugversionen gehören zur ausgegebenen Projektbasis.
3. CM4 übernimmt `HAL_Init()`, die zur Boardversorgung passende Spannungs-/Taktinitialisierung und danach GPIO, HAL-Zeitbasis und RTOS-Initialisierung. Als ST-Referenz dient [STM32CubeH7, BootCM4_CM7Gated](https://github.com/STMicroelectronics/STM32CubeH7/tree/8ec3113d4039d30ba9dc2bf0cb6eefc756b48d06/Projects/NUCLEO-H745ZI-Q/Templates/BootCM4_CM7Gated). Dort initialisiert CM4 bereits Systemtakt und Spannung, gibt anschließend jedoch CM7 frei. **Diese Freigabe `HAL_RCCEx_EnableBootCore(RCC_BOOT_C1)` entfällt hier vollständig.** Die Referenz ist auf STM32H745 ausgerichtet; MCU-, Versorgungs- und Taktparameter müssen zum H755-Projekt passen. ST nennt die NUCLEO-H745ZI-Q-Projekte im [H755-Kompatibilitätshinweis](https://github.com/STMicroelectronics/STM32CubeH7/blob/8ec3113d4039d30ba9dc2bf0cb6eefc756b48d06/Projects/NUCLEO-H755ZI-Q/readme.txt) als kompatibel. Das ersetzt die Boardprüfung nicht.
4. Prüfen Sie einen Kaltstart nach aus- und wieder eingeschalteter Versorgung, auch ohne Debugger: CM4 erreicht `main()` und den Kernel, während CM7 gesperrt bleibt. Ein nur durch den Debugger gesetzter Programmeinstieg ist kein Nachweis der Bootkonfiguration. Für diese Aufgabe sind weder ein CM7-Bootloader noch ein zweites Image nötig.

## CubeMX-Konfiguration für die zwei Threads

1. Öffnen Sie die `.ioc`-Datei im **CM4-Kontext**. Ordnen Sie **PE1 / LD2** und **PB0 / LD1** dem CM4 zu und wählen Sie `GPIO_Output`. Unter **System Core > GPIO**: Output Push Pull, No pull-up and no pull-down, Low speed, Ausgangspegel Low. Prüfen Sie Pinbelegung und aktive Polarität gegen UM2408 sowie den Aufruf der passenden `MX_GPIO_Init()` im CM4-Start.
2. Unter **Middleware and Software Packs > FREERTOS**: Interface **CMSIS_V2**. Unter **Config parameters / Configuration**: Präemption an, Tickrate **1000 Hz**, Ticktyp **32 Bit**, Tickless Idle aus, dynamische Allokation mit `heap_4` und zunächst **16384 Byte** Heap. Prüfen Sie die tatsächliche `FreeRTOSConfig.h`, die CMSIS-Prioritätsabbildung und den Cortex-M4-Port. Der Heapwert ist ein Ausgangswert, kein Mindestnachweis für beliebige Projekte.
3. Unter **Tasks and Queues** den Default-Thread entfernen und genau zwei Anwendungsthreads erzeugen:

| Task Name | Entry Function | CMSIS-Priorität | Dialog bei Words | CMSIS `.stack_size` |
|---|---|---|---:|---:|
| `Heartbeat` | `StartHeartbeatTask` | `osPriorityNormal` | 128 | 512 Byte |
| `HeavyMath` | `StartHeavyMathTask` | `osPriorityBelowNormal` | 256 | 1024 Byte |

   Wenn der Dialog Byte erwartet, direkt 512 bzw. 1024 eintragen. CMSIS `stack_size` ist immer in Byte; der Generator kann z. B. `128 * 4` ausgeben. Dynamische Allokation und Argument `NULL` wählen. Die beiden Stackgrößen müssen am Ziel geprüft werden. Kernelobjekte, Idle und Handlerstack gehören zusätzlich in die RAM-Bilanz.
4. Unter **Project Manager > Project** CMake als Toolchain wählen und unter **Code Generator** *Keep User Code when re-generating* aktivieren. Nur das CM4-Ziel bauen und debuggen. Die tatsächlichen Menübezeichnungen können mit der Werkzeugversion variieren; die erzeugten Werte sind die Kontrolle.

## Gemeinsamen SysTick im vorhandenen Projekt erhalten

Unter **System Core > SYS > Timebase Source** wird **SysTick** gewählt; die HAL-Zeitbasis bleibt bei 1 kHz. Es wird keine generierte Timerzeitbasis eingebunden und kein TIM6 gestartet. Auch der Kernel verwendet 1 kHz, bei ausgeschaltetem Tickless Idle. Ein funktionierendes FreeRTOS benötigt weiterhin seine Kernelinterrupts; die Anwendung selbst programmiert keine Interruptlogik.

Die gemeinsame Zeitbasis wird **in den bestehenden Projektdateien** eingerichtet. Es gibt keine zusätzliche `lab03_*.c`-Datei. Für den ST-CMSIS-RTOS2-Adapter mit dem Schalter `USE_CUSTOM_SYSTICK_HANDLER_IMPLEMENTATION`:

1. Setzen Sie den Schalter für alle CM4-Quellen einschließlich des CMSIS-Adapters. Beispielsweise in der **CM4-`CMakeLists.txt` vor dem Einbinden der generierten Unterverzeichnisse**:

```cmake
add_compile_definitions(USE_CUSTOM_SYSTICK_HANDLER_IMPLEMENTATION=1)
```

   Der Schalter unterdrückt die adaptereigene Definition von `SysTick_Handler`. Prüfen Sie, dass der verwendete Adapter diesen Schalter unterstützt. Die CM4-Konfiguration muss `INCLUDE_xTaskGetSchedulerState = 1` enthalten.
2. Es darf keine Makroabbildung `xPortSysTickHandler` auf `SysTick_Handler` bestehen. Die generierte SVC-/PendSV-Anbindung und der SysTick-Vektoreintrag bleiben erhalten. Es gibt **genau eine C-Definition** von `SysTick_Handler`.
3. Ergänzen bzw. ersetzen Sie diese Definition in der vorhandenen CM4-Interruptdatei, üblicherweise `stm32h7xx_it.c`. Platzieren Sie Includes und die Definition möglichst in erhaltenen Benutzerbereichen. Der folgende Baustein steht vollständig hier in der Anleitung:

```c
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

extern void xPortSysTickHandler(void);

void SysTick_Handler(void)
{
    (void)SysTick->CTRL;
    HAL_IncTick();
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        xPortSysTickHandler();
    }
}
```

   Der HAL-Tick läuft bereits während der Initialisierung. Nach dem Schedulerstart wird zusätzlich der Kernel-Tick bedient. Die Abfrage und das Lesen von `SysTick->CTRL` entsprechen dem ST-Adapter; die gemeinsame Variante ergänzt `HAL_IncTick`. Zusätzliche vorhandene Tickaufrufe dürfen nicht doppelt erhalten bleiben.
4. Prüfen Sie vor dem Kernelstart den Fortschritt von `HAL_GetTick()` und danach den von HAL-Millisekunden und Kernelticks. Eine erneute CubeMX-Generierung muss die gemeinsame Zeitbasis erhalten; andernfalls die dokumentierten Änderungen wiederherstellen. Weder ein zweiter Handler noch die alte Timerzeitbasis darf aktiviert werden.

## Generierte Threadfunktionen und Startfolge

Suchen Sie im CM4-Quellbereich nach `StartHeartbeatTask` und `StartHeavyMathTask`, je nach Generator in `freertos.c`, `app_freertos.c` oder `main.c`. Die beiden Schleifen werden direkt in den zugehörigen `USER CODE`-Bereichen geschrieben. Die vollständigen Musterfunktionen stehen ausschließlich im `solution`-Profil des Übungsblatts. Es gibt keine gesonderte studentische Quelldatei und keinen Aufruf eines Labor-Starthelfers.

Der Generator legt mit `osThreadNew` beide Threads an. Prüfen Sie deren tatsächlich generierte Handles direkt danach und vor `osKernelStart()` auf `NULL`, mit `Error_Handler()` im Fehlerfall. Bei den vorgeschlagenen Tasknamen heißen sie üblicherweise `HeartbeatHandle` und `HeavyMathHandle`; der Prüfbaustein steht im Blatt. Lassen Sie genau eine Kernelinitialisierung, die zwei Threadanlagen und genau einen Kernelstart bestehen. Eine unerwartete Rückkehr aus dem Kernelstart gehört in den Fehlerpfad.

Vor dem Termin: Projekt erzeugen, CM4-Build und Kaltstart prüfen, beide Eintrittsfunktionen erreichen. Nach eigener Implementierung soll LD2 regelmäßig und LD1 abhängig von der Rechenblockdauer umschalten. Die anfänglichen 10 Millionen Iterationen können für die ausgegebene Takt-/Buildkonfiguration angepasst werden, wenn LD1 schlecht sichtbar ist. Es gibt für HeavyMath keinen festen Periodenvertrag.

Der optionale Prioritätsversuch wird in **CubeMX > FREERTOS > Tasks and Queues** durchgeführt: HeavyMath auf AboveNormal, erneut erzeugen, erhaltene Benutzerbereiche prüfen, bauen und starten. Danach BelowNormal wiederherstellen. Es laufen weiterhin genau zwei Anwendungsthreads. Der Kernel besitzt zusätzlich seinen internen Idle-Thread und gegebenenfalls weitere konfigurierte Kernel-Tasks.

Das Repository enthält kein generiertes Firmwaregesamtprojekt. Ein Hosttest der gedruckten Codebausteine ersetzt weder den ARM-Build noch den tatsächlichen Kaltstart und Boardlauf. Ohne funktionierende Hardware bleiben die Papieraufgaben lösbar; ein nicht ausgeführter Boardtest wird benannt.
