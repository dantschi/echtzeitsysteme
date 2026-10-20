/*
 * Prepared support for lab 3 task 6; students supply the thread bodies.
 * NUCLEO-H755ZI-Q, CM4 only, CMSIS-RTOS2/FreeRTOS. Not a complete project.
 * The CM4 project supplies standalone startup, clocks, GPIO and kernel
 * configuration. lab03_systick.c supplies the shared HAL/kernel tick.
 * CM7 stays boot-disabled. LD2 = PE1, LD1 = PB0 (MB1363).
 * No button, EXTI, application ISR or peripheral timer is used.
 */
#include "lab03_rtos.h"
#include "main.h"

static int started;
volatile uint32_t lab03_heartbeat_edges;
volatile uint32_t lab03_accumulator;

_Noreturn void Lab03_Fail(void)
{
    Error_Handler();
    /* Also stop if the project handler unexpectedly returns. */
    for (;;) {}
}

void Lab03_Start(void)
{
    static const osThreadAttr_t heartbeat_attr = {
        .name = "Heartbeat", .stack_size = 512U,
        .priority = osPriorityNormal
    };
    static const osThreadAttr_t math_attr = {
        .name = "HeavyMath", .stack_size = 1024U,
        .priority = osPriorityBelowNormal /* Experiment: AboveNormal. */
    };
    if (started || osKernelGetState() != osKernelReady ||
        osKernelGetTickFreq() != 1000U) {
        Lab03_Fail();
    }
    started = 1;
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    if (osThreadNew(Lab03_Heartbeat, NULL, &heartbeat_attr) == NULL ||
        osThreadNew(Lab03_HeavyMath, NULL, &math_attr) == NULL) {
        Lab03_Fail();
    }
}

void Lab03_ToggleHeartbeat(void)
{
    ++lab03_heartbeat_edges;
    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
}

void Lab03_ToggleMath(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}
