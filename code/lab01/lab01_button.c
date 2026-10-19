#include "lab01_button.h"
#if !defined(LAB01_BUTTON_Pin) || !defined(LAB01_BUTTON_GPIO_Port) || \
    !defined(LAB01_LED_Pin) || !defined(LAB01_LED_GPIO_Port) || \
    !defined(LAB01_STATUS_Pin) || !defined(LAB01_STATUS_GPIO_Port)
#error "Configure the three onboard pin labels from the preparation guide"
#endif
volatile uint32_t lab01_presses;
static uint32_t candidate_since, status_since;
static int candidate, stable, initialized;
void Lab01_ButtonInit(void)
{
    if (initialized || HAL_GetTickFreq() != HAL_TICK_FREQ_1KHZ) {
        Error_Handler(); for (;;) {}
    }
    candidate = stable = HAL_GPIO_ReadPin(LAB01_BUTTON_GPIO_Port,
                                         LAB01_BUTTON_Pin) == GPIO_PIN_SET;
    candidate_since = status_since = HAL_GetTick();
    Lab01_ButtonLed(0);
    HAL_GPIO_WritePin(LAB01_STATUS_GPIO_Port, LAB01_STATUS_Pin, GPIO_PIN_RESET);
    initialized = 1;
}
int Lab01_ButtonPressed(void)
{
    const int raw = HAL_GPIO_ReadPin(LAB01_BUTTON_GPIO_Port,
                                     LAB01_BUTTON_Pin) == GPIO_PIN_SET;
    const uint32_t now = HAL_GetTick();
    if (raw != candidate) { candidate = raw; candidate_since = now; }
    if ((uint32_t)(now - candidate_since) >= 20U) { stable = candidate; }
    return stable;
}
void Lab01_ButtonLed(int on)
{
    HAL_GPIO_WritePin(LAB01_LED_GPIO_Port, LAB01_LED_Pin,
                      on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
void Lab01_ButtonRunOnce(void)
{
    if (!initialized) { Error_Handler(); for (;;) {} }
    Lab01_ButtonStep();
    const uint32_t now = HAL_GetTick();
    if ((uint32_t)(now - status_since) >= 250U) {
        status_since = now;
        HAL_GPIO_TogglePin(LAB01_STATUS_GPIO_Port, LAB01_STATUS_Pin);
    }
}
