#ifndef LAB09_TIMER_H
#define LAB09_TIMER_H
#include "main.h"
#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "Timer source belongs only to CM4"
#endif
/* Add ONE call inside the EXISTING HAL_TIM_PeriodElapsedCallback.
 * Keep its existing HAL timebase handling intact. TIM2 is generated at 100 Hz. */
void Lab09_CM4_TimerCallback(TIM_HandleTypeDef *timer);
#endif
