#ifndef LAB01_DEMO_H
#define LAB01_DEMO_H

#include "stm32h7xx_hal.h"

#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "Build lab01_demo only for the Cortex-M4 application."
#endif

typedef enum {
    LAB01_BASELINE = 0,
    LAB01_POLL_CONSTANT,
    LAB01_POLL_VARIABLE,
    LAB01_TIMER
} Lab01_Mode;

typedef struct {
    uint32_t main_steps;       /* Main owns this counter. */
    uint32_t last_iterations;  /* Iterations, not a measured duration. */
    uint32_t timer_callbacks;  /* TIM2 callback owns this counter. */
} Lab01_Diagnostics;

extern volatile Lab01_Diagnostics lab01_diag;

/* Call once, after generated GPIO/TIM initialization and dual-core boot.
   LD2/PE1 and TIM2 must belong exclusively to CM4. HAL tick must advance.
   Returns 1 on success, 0 for invalid/repeated setup or timer start failure.
   The selected mode remains fixed until reset. */
int Lab01_Init(Lab01_Mode selected, TIM_HandleTypeDef *timer);
void Lab01_Step(void);
/* Delegate once from the existing HAL_TIM_PeriodElapsedCallback.
   Preserve existing handling for any HAL timebase timer. */
void Lab01_TimerCallback(TIM_HandleTypeDef *htim);

#endif
