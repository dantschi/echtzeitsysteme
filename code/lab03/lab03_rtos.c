/*
 * Prepared support for lab 3 task 6; students supply the thread bodies.
 * NUCLEO-H755ZI-Q, CM4 only, CMSIS-RTOS2/FreeRTOS. Not a complete project.
 * The CM4 project must provide standalone startup, clocks, GPIO, EXTI,
 * HAL timebase and kernel configuration. CM7 stays boot-disabled.
 * LD2 = PE1, LD1 = PB0, user button = PC13 (MB1363).
 * Configure EXTI15_10 at a kernel-call-permitted priority and keep its
 * NVIC enable disabled until Lab03_Event starts. Use a 32-bit tick.
 */
#include "lab03_rtos.h"
#include "main.h"

static osThreadId_t event_thread;
static int started;
volatile uint32_t lab03_heartbeat_edges;
volatile uint32_t lab03_target_misses;
volatile uint32_t lab03_irq_errors;
volatile uint32_t lab03_event_responses;
volatile uint32_t lab03_accumulator;

_Noreturn void Lab03_Fail(void)
{
    Error_Handler();
    /* Also stop if the project handler unexpectedly returns. */
    for (;;) {}
}

void Lab03_Start(void)
{
    static const osThreadAttr_t event_attr = {
        .name = "Event", .stack_size = 1024U,
        .priority = osPriorityHigh
    };
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
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    event_thread = osThreadNew(Lab03_Event, NULL, &event_attr);
    if (event_thread == NULL ||
        osThreadNew(Lab03_Heartbeat, NULL, &heartbeat_attr) == NULL ||
        osThreadNew(Lab03_HeavyMath, NULL, &math_attr) == NULL) {
        Lab03_Fail();
    }
}

void Lab03_ToggleHeartbeat(void)
{
    ++lab03_heartbeat_edges;
    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
}

void Lab03_ToggleEvent(void)
{
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}

void Lab03_WaitHeartbeat(uint32_t *next)
{
    if (next == NULL) {
        Lab03_Fail();
    }
    *next += LAB03_HEARTBEAT_TICKS;  /* Defined unsigned wrap. */
    const uint32_t now = osKernelGetTickCount();
    const uint32_t remaining = *next - now;
    if (remaining == 0u || remaining > UINT32_C(0x7fffffff)) {
        ++lab03_target_misses;
        *next = now + LAB03_HEARTBEAT_TICKS;
    }
    osStatus_t status = osDelayUntil(*next);
    if (status == osErrorParameter || status == osError) {
        /*
         * Period is validated. The target can expire between the read
         * and wait call (including the adapter's own tick read).
         * Resynchronise once and wait, never do an immediate extra toggle.
         */
        ++lab03_target_misses;
        *next = osKernelGetTickCount() + LAB03_HEARTBEAT_TICKS;
        status = osDelayUntil(*next);
    }
    if (status != osOK) {
        Lab03_Fail();
    }
}

void Lab03_EnableButtonIRQ(void)
{
    if (event_thread == NULL || osKernelGetState() != osKernelRunning) {
        Lab03_Fail();
    }
    /*
     * Startup policy: discard button edges pending before this point.
     * No other interrupt source may use this shared IRQ in this lab.
     */
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
    HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void Lab03_ButtonIRQ(uint16_t gpio_pin)
{
    if (gpio_pin != GPIO_PIN_13) {
        return;
    }
    if (event_thread == NULL || osKernelGetState() != osKernelRunning) {
        ++lab03_irq_errors;
        return;
    }
    const uint32_t flags = osThreadFlagsSet(event_thread, LAB03_BUTTON_FLAG);
    if ((flags & osFlagsError) != 0u) {
        ++lab03_irq_errors;
    }
}
