#ifndef LAB03_TANK_H
#define LAB03_TANK_H

#include <stdint.h>
#include "cmsis_os2.h"

#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "Build lab03_tank only for the Cortex-M4 application."
#endif

#define LAB03_TANK_FLAG UINT32_C(1)
#define LAB03_TANK_SAMPLE_TICKS UINT32_C(200)

/* Prepared setup: call once in osKernelReady, before osKernelStart().
   Requires preemption, 32-bit/1-kHz kernel ticks and a CM4 TANK_LED output.
   Creates both threads; no separate generated copies of them are needed. */
void Lab03_TankStart(void);

/* Implement both entry functions in the student's tank_threads.c. */
void Lab03_TankSampler(void *argument);
void Lab03_TankAlarm(void *argument);

uint32_t Lab03_TankRead(void); /* Only the sampler calls this. */
void Lab03_TankLed(int on);   /* Only the alarm thread calls this. */
/* Initialize *next with osKernelGetTickCount() before the sampler loop.
   Uses the previous target + 200 ticks; resynchronizes after a missed
   target. Targets and unobserved delays must stay below half the range. */
void Lab03_TankWaitNext(uint32_t *next);
_Noreturn void Lab03_TankFail(void);

/* Assigned during startup, immutable while threads run. */
extern osThreadId_t lab03_tank_alarm_handle;

/* Diagnostic observations, not a data handoff. Read only with CM4 halted.
   Single writers: sampler -> level/requests/target_misses;
   alarm -> responses. Unsigned counters wrap modulo 2^32. */
extern volatile uint32_t lab03_tank_level;
extern volatile uint32_t lab03_tank_requests;
extern volatile uint32_t lab03_tank_responses;
extern volatile uint32_t lab03_tank_target_misses;

#endif
