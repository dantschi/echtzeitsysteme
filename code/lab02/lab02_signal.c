/* Prepared support for lab 2. The student supplies Lab02_SignalStep(). */
#include "lab02_signal.h"
#include "main.h"

#if !defined(LAB_MARKER_GPIO_Port) || !defined(LAB_MARKER_Pin) || \
    !defined(LAB_STATUS_GPIO_Port) || !defined(LAB_STATUS_Pin)
#error "Configure CM4 LED outputs labelled LAB_MARKER and LAB_STATUS."
#endif

volatile uint32_t lab02_signal_checks;

static void SignalFail(void)
{
    Error_Handler();
    for (;;) {}
}

void Lab02_SignalWrite(int on)
{
    HAL_GPIO_WritePin(LAB_MARKER_GPIO_Port, LAB_MARKER_Pin,
                      on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Lab02_SignalPause(uint32_t ms)
{
    if (!Lab02_SleepAfterStep(ms)) {
        SignalFail();
    }
}

static void CounterStep(void)
{
    ++lab02_signal_checks;
}

static void StatusStep(void)
{
    HAL_GPIO_TogglePin(LAB_STATUS_GPIO_Port, LAB_STATUS_Pin);
    Lab02_SignalPause(700U);
}

int Lab02_SignalInit(void)
{
    void (*const steps[LAB02_N])(void) = {
        Lab02_SignalStep, CounterStep, StatusStep
    };
    if (!Lab02_InitSteps(steps)) {
        return 0;
    }
    lab02_signal_checks = 0U;
    HAL_GPIO_WritePin(LAB_STATUS_GPIO_Port, LAB_STATUS_Pin, GPIO_PIN_RESET);
    return 1;
}
