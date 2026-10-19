#include "main.h"
#include "lab08_wwdg.h"
/* Optional separate watchdog display using onboard LEDs only:
 * LAB_ENABLE = LD1/PB0, LAB_REFRESH = LD2/PE1, LAB_EWI = LD3/PB14.
 * Configure CM4 push-pull outputs, initially LOW; verify board revision.
 * These LEDs do not certify an external actuator's state during reset.
 * The current health exercise uses lab08_health.c instead. */
#if !defined(LAB_ENABLE_Pin) || !defined(LAB_ENABLE_GPIO_Port) || \
    !defined(LAB_REFRESH_Pin) || !defined(LAB_REFRESH_GPIO_Port) || \
    !defined(LAB_EWI_Pin) || !defined(LAB_EWI_GPIO_Port)
#error "Configure all three labelled GPIO outputs in the CM4 CubeMX project"
#endif
void Lab08_OutputOff(void)
{
    HAL_GPIO_WritePin(LAB_ENABLE_GPIO_Port, LAB_ENABLE_Pin, GPIO_PIN_RESET);
}
void Lab08_OutputOn(void)
{
    HAL_GPIO_WritePin(LAB_ENABLE_GPIO_Port, LAB_ENABLE_Pin, GPIO_PIN_SET);
}
void Lab08_MarkRefresh(void)
{
    HAL_GPIO_TogglePin(LAB_REFRESH_GPIO_Port, LAB_REFRESH_Pin);
}
void Lab08_MarkEWI(void)
{
    HAL_GPIO_WritePin(LAB_EWI_GPIO_Port, LAB_EWI_Pin, GPIO_PIN_SET);
}
