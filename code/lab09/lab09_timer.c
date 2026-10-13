#include "lab09_timer.h"
#include "lab09_capstone.h"
extern TIM_HandleTypeDef htim2; /* Generated, owned and initialized on CM4. */
static uint32_t sample;
int Lab09_CM4_StartEvents(void)
{
    sample = 0u;
    return HAL_TIM_Base_Start_IT(&htim2) == HAL_OK;
}
int Lab09_CM4_StopEvents(void)
{
    return HAL_TIM_Base_Stop_IT(&htim2) == HAL_OK;
}
void Lab09_CM4_TimerCallback(TIM_HandleTypeDef *timer)
{
    if (timer != NULL && timer->Instance == TIM2) {
        if (sample < LAB09_MAX_EVENTS * 10u) { sample += 10u; }
        Lab09_CM4_OnEvent(sample);
    }
}
