#include "lab01_demo.h"
#include <stddef.h>

volatile Lab01_Diagnostics lab01_diag;
/* Single-core setup precedes the first TIM2 callback; mode is fixed thereafter. */
static volatile int initialized;
static volatile Lab01_Mode mode;

static void BusyLoad(uint32_t iterations)
{
    volatile uint32_t remaining = iterations;
    while (remaining > 0U) {
        --remaining;
    }
}

int Lab01_Init(Lab01_Mode selected, TIM_HandleTypeDef *timer)
{
    if (initialized || selected < LAB01_BASELINE || selected > LAB01_TIMER) {
        return 0;
    }
    if (selected == LAB01_TIMER &&
        (timer == NULL || timer->Instance != TIM2)) {
        return 0;
    }
    mode = selected;
    lab01_diag.main_steps = 0U;
    lab01_diag.last_iterations = 0U;
    lab01_diag.timer_callbacks = 0U;
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
    if (mode == LAB01_TIMER) {
        /* Start a fresh observation period; discard the initialization update. */
        __HAL_TIM_SET_COUNTER(timer, 0U);
        __HAL_TIM_CLEAR_FLAG(timer, (uint32_t)TIM_FLAG_UPDATE);
    }
    initialized = 1;
    if (mode == LAB01_TIMER && HAL_TIM_Base_Start_IT(timer) != HAL_OK) {
        initialized = 0;
        return 0;
    }
    return 1;
}

void Lab01_Step(void)
{
    if (!initialized) {
        return;
    }
    ++lab01_diag.main_steps;
    uint32_t iterations = 0U;
    if (mode == LAB01_POLL_CONSTANT) {
        iterations = 800000U;
    } else if (mode == LAB01_POLL_VARIABLE || mode == LAB01_TIMER) {
        iterations = (lab01_diag.main_steps & 1U) ? 400000U : 1600000U;
    }
    lab01_diag.last_iterations = iterations;
    if (mode != LAB01_TIMER) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
    }
    BusyLoad(iterations);
    if (mode != LAB01_TIMER) {
        HAL_Delay(500U);
    }
}

void Lab01_TimerCallback(TIM_HandleTypeDef *htim)
{
    if (initialized && mode == LAB01_TIMER &&
        htim != NULL && htim->Instance == TIM2) {
        ++lab01_diag.timer_callbacks;
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
    }
}
