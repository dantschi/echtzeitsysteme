#ifndef LAB03_RTOS_H
#define LAB03_RTOS_H

#include <stdint.h>
#include "cmsis_os2.h"

#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "Build lab03_rtos only for the Cortex-M4 application."
#endif

#define LAB03_BUTTON_FLAG UINT32_C(1)
#define LAB03_HEARTBEAT_TICKS UINT32_C(500)

/* Prepared CM4-only setup, preemption, 32-bit/1-kHz kernel ticks.
   Call once in osKernelReady, before osKernelStart(). Creates all three
   threads; do not generate additional copies of them in CubeMX. */
void Lab03_Start(void);

/* Implement these entry functions in threads_student.c. */
void Lab03_Heartbeat(void *argument);
void Lab03_HeavyMath(void *argument);
void Lab03_Event(void *argument);

void Lab03_ToggleHeartbeat(void); /* LD2/PE1, heartbeat thread only. */
void Lab03_ToggleEvent(void);     /* LD1/PB0, event thread only. */
/* Initialize *next with osKernelGetTickCount() before the loop. Advances
   the previous target by 500 ticks; resynchronizes after a missed target.
   Unobserved delays must remain below half the 32-bit tick range. */
void Lab03_WaitHeartbeat(uint32_t *next);
/* Event thread calls this once before its first flag wait. Discards old
   button edges and enables EXTI15_10; no flag clear after enabling. */
void Lab03_EnableButtonIRQ(void);
_Noreturn void Lab03_Fail(void);

/* Forward the existing CM4 HAL_GPIO_EXTI_Callback() here. */
void Lab03_ButtonIRQ(uint16_t gpio_pin);

/* Read together only with CM4 halted; volatile is not synchronization.
   Writers: heartbeat -> edges/target_misses; event -> responses;
   button ISR -> irq_errors; heavy math -> accumulator. */
extern volatile uint32_t lab03_heartbeat_edges;
extern volatile uint32_t lab03_target_misses;
extern volatile uint32_t lab03_irq_errors;
extern volatile uint32_t lab03_event_responses;
extern volatile uint32_t lab03_accumulator;

#endif
