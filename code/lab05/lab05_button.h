#ifndef LAB05_BUTTON_H
#define LAB05_BUTTON_H
#include <stdint.h>
#include "cmsis_os2.h"
#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "Button queue exercise requires CM4"
#endif
typedef struct { uint32_t seq, tick; } Lab05_Press;
extern osMessageQueueId_t lab05_press_queue;
void Lab05_ButtonStart(void); /* Once in kernel Ready. */
void Lab05_ButtonIRQ(uint16_t pin); /* Existing HAL callback delegates. */
void Lab05_ButtonLed(int on);
void Lab05_ButtonCheck(osStatus_t status); /* Thread context only. */
void Lab05_QueueButton(void); /* Student ISR callback; no blocking. */
void Lab05_PulseWorker(void *argument); /* Student thread. */
/* Single writers per prefix. Read with source stopped and CM4 halted.
   Unsigned counters wrap; these are diagnostics, not lossless hardware counts. */
extern volatile uint32_t lab05_irq_offered, lab05_irq_accepted;
extern volatile uint32_t lab05_irq_dropped, lab05_irq_errors;
extern volatile uint32_t lab05_worker_handled, lab05_worker_ignored;
extern volatile uint32_t lab05_worker_pulses;
#endif
