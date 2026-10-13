#include "main.h"
#include "lab08_wwdg.h"
/* CubeMX labels, not assumed Nucleo LED/header pin numbers.
 * GPIO output push-pull, initial LOW. LAB_ENABLE has external pull-down,
 * isolated indicator only. LAB_REFRESH toggles at a refresh attempt,
 * LAB_EWI rises in the warning callback. No connected actuator. */
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
