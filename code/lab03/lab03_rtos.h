#ifndef LAB03_RTOS_H
#define LAB03_RTOS_H

#include <stdint.h>
#include "cmsis_os2.h"

#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "Build lab03_rtos only for the Cortex-M4 application."
#endif

#define LAB03_HEARTBEAT_TICKS UINT32_C(500)
#define LAB03_MATH_ITERATIONS UINT32_C(10000000)

/* Prepared CM4-only setup, preemption, 32-bit/1-kHz kernel ticks.
   Call once in osKernelReady, before osKernelStart(). Creates both
   threads; do not generate additional copies of them in CubeMX. */
void Lab03_Start(void);

/* Implement these entry functions in threads_student.c. */
void Lab03_Heartbeat(void *argument);
void Lab03_HeavyMath(void *argument);

void Lab03_ToggleHeartbeat(void); /* LD2/PE1, heartbeat thread only. */
void Lab03_ToggleMath(void);      /* LD1/PB0, math thread only. */
_Noreturn void Lab03_Fail(void);

/* Read together only with CM4 halted; volatile is not synchronization.
   Writers: heartbeat -> heartbeat_edges; heavy math -> accumulator. */
extern volatile uint32_t lab03_heartbeat_edges;
extern volatile uint32_t lab03_accumulator;

#endif
