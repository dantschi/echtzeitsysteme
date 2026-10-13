#include "main.h"
#include "cmsis_os2.h"
#include "lab08_wwdg.h"
#if USE_HAL_WWDG_REGISTER_CALLBACKS != 0
#error "This example owns the weak EWI callback; select callback registration 0"
#endif
static WWDG_HandleTypeDef watchdog;
static volatile int started;
volatile uint32_t lab08_wwdg_refreshes;
volatile uint32_t lab08_wwdg_error;
volatile uint32_t lab08_wwdg_ewi;

static void Fail(uint32_t error)
{
    lab08_wwdg_error = error;
    Lab08_OutputOff();
    /* No refresh. If already started, WWDG will reset, even while here. */
    Error_Handler();
    for (;;) {}
}
void Lab08_WWDG_IRQHandler(void)
{
    if (started) { HAL_WWDG_IRQHandler(&watchdog); }
}
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *handle)
{
    if (handle == &watchdog) {
        Lab08_OutputOff();
        Lab08_MarkEWI();
        lab08_wwdg_ewi = 1u;
        /* No kernel, flash, delay, logging or refresh. */
    }
}
void Lab08_WWDG_Run(void *argument)
{
    if (argument == NULL || started || osKernelGetState() != osKernelRunning) {
        Fail(1u);
    }
    const Lab08_WWDG_Config config = *(const Lab08_WWDG_Config *)argument;
    if ((config.mode != LAB08_WWDG_EARLY && config.mode != LAB08_WWDG_VALID &&
         config.mode != LAB08_WWDG_LATE) ||
        config.irq_priority >= (1u << __NVIC_PRIO_BITS) ||
        osKernelGetTickFreq() != 1000u || HAL_RCC_GetPCLK1Freq() != 100000000u) {
        Fail(2u);
    }
    /* GPIOs must already have been initialized in CM4, low and clocked.
     * Other watchdog instances/option-byte starts are excluded by project. */
    Lab08_OutputOff();
    __HAL_RCC_WWDG2_CLK_ENABLE();
    HAL_NVIC_SetPriority(WWDG_IRQn, config.irq_priority, 0u);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
    watchdog.Instance = WWDG2;
    watchdog.Init.Prescaler = WWDG_PRESCALER_8;
    watchdog.Init.Window = 80u;
    watchdog.Init.Counter = 127u;
    watchdog.Init.EWIMode = WWDG_EWI_ENABLE;
    if (config.mode == LAB08_WWDG_VALID) { Lab08_OutputOn(); }
    started = 1; /* Valid handle before interrupts can reach the callback. */
    if (HAL_WWDG_Init(&watchdog) != HAL_OK) { Fail(3u); }
    uint32_t next = osKernelGetTickCount();
    if (config.mode == LAB08_WWDG_EARLY) {
        if (osDelay(5u) != osOK) { Fail(4u); }
        Lab08_MarkRefresh();
        (void)HAL_WWDG_Refresh(&watchdog); /* Deliberate early-window fault. */
        Fail(5u); /* Reached only if experiment did not reset as expected. */
    }
    if (config.mode == LAB08_WWDG_LATE) {
        if (osDelay(25u) != osOK) { Fail(6u); }
        Fail(7u); /* A running, unfrozen watchdog should reset before this. */
    }
    for (uint32_t round = 0u; round < 10u; ++round) {
        next += 18u;
        if (osDelayUntil(next) != osOK) { Fail(8u); }
        /* Preserve the incoming mask. Keep this local IRQ-masked section
         * short and measure its maximum duration in a hardware timing study.
         * The hardware counter keeps running while interrupts are masked. */
        const uint32_t mask = __get_PRIMASK();
        __disable_irq();
        const uint32_t counter = watchdog.Instance->CR & 0x7Fu;
        if (lab08_wwdg_ewi != 0u || counter > 80u || counter <= 64u) {
            __set_PRIMASK(mask);
            Fail(9u);
        }
        Lab08_MarkRefresh();
        HAL_StatusTypeDef status = HAL_WWDG_Refresh(&watchdog);
        __set_PRIMASK(mask);
        if (status != HAL_OK) { Fail(10u); }
        ++lab08_wwdg_refreshes;
    }
    Lab08_OutputOff();
    /* End this finite experiment by intentionally allowing a late reset. */
    for (;;) {
        if (osDelay(1u) != osOK) { Fail(11u); }
    }
}
