/* Prepared CM4-only timebase for lab 3: one 1-kHz SysTick for HAL and
 * FreeRTOS. No TIM6 or application timer interrupt is needed.
 * Use the ST CMSIS-RTOS2 adapter with the target-wide compile definition
 * USE_CUSTOM_SYSTICK_HANDLER_IMPLEMENTATION=1. Remove any generated
 * SysTick_Handler and xPortSysTickHandler -> SysTick_Handler macro alias.
 * Retain the generated SVC/PendSV routing. See the project guide.
 */
#include "lab03_rtos.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

#if USE_CUSTOM_SYSTICK_HANDLER_IMPLEMENTATION != 1
#error "Set USE_CUSTOM_SYSTICK_HANDLER_IMPLEMENTATION=1 for the CM4 target."
#endif
#if defined(xPortSysTickHandler)
#error "Remove the xPortSysTickHandler macro alias; call the port handler directly."
#endif

_Static_assert(configTICK_RATE_HZ == 1000U, "Use a 1-kHz tick for HAL and RTOS.");
_Static_assert(configUSE_TICKLESS_IDLE == 0, "This shared timebase is not tickless.");
_Static_assert(sizeof(TickType_t) == sizeof(uint32_t), "Use 32-bit RTOS ticks.");

extern void xPortSysTickHandler(void);

void SysTick_Handler(void)
{
    (void)SysTick->CTRL; /* Clear COUNTFLAG, as in the ST CMSIS adapter. */
    HAL_IncTick();      /* Runs during HAL initialization as well. */
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        xPortSysTickHandler();
    }
}
