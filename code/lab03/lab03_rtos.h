#ifndef LAB03_RTOS_H
#define LAB03_RTOS_H

#include <stdint.h>
#include "cmsis_os2.h"

/* Call after creating all threads, before osKernelStart(). */
void Lab03_BindEvent(osThreadId_t event_handle);

/* Call from generated CM4 thread entry functions. These do not return. */
void Lab03_Heartbeat(void *argument);
void Lab03_HeavyMath(void *argument);
void Lab03_Event(void *argument);

/* Call from the existing HAL_GPIO_EXTI_Callback(). */
void Lab03_ButtonIRQ(uint16_t gpio_pin);

/* Debugger observations, not timing guarantees. Each has one writer. */
extern volatile uint32_t lab03_target_misses;
extern volatile uint32_t lab03_irq_errors;
extern volatile uint32_t lab03_event_responses;
extern volatile uint32_t lab03_accumulator;

#endif
