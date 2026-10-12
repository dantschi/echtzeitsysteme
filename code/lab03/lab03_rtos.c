/*
 * VL3 teaching module: NUCLEO-H755ZI-Q, CM4, CMSIS-RTOS2/FreeRTOS.
 * Not a complete firmware project. CubeMX must provide clocks, GPIO,
 * EXTI, HAL timebase, kernel configuration and dual-core boot.
 * LD2 = PE1, LD1 = PB0, user button = PC13 (MB1363).
 * Configure EXTI15_10 at a kernel-call-permitted priority and keep its
 * NVIC enable disabled until Lab03_Event starts. Use a 32-bit tick.
 */
#include "lab03_rtos.h"
#include "main.h"

static osThreadId_t event_thread;
volatile uint32_t lab03_target_misses;
volatile uint32_t lab03_irq_errors;
volatile uint32_t lab03_event_responses;
volatile uint32_t lab03_accumulator;

static void LabFail(void)
{
    Error_Handler();
    /* Also stop if the project handler unexpectedly returns. */
    for (;;) {}
}

/* Same conversion as the lecture; zero and ambiguous distances rejected. */
static uint32_t LabMsToTicks(uint32_t ms)
{
    const uint32_t frequency = osKernelGetTickFreq();
    const uint64_t ticks =
        ((uint64_t)ms * frequency + 999u) / 1000u;
    if (ms == 0u || frequency == 0u ||
        ticks == 0u || ticks > UINT32_C(0x7fffffff)) {
        LabFail();
    }
    return (uint32_t)ticks;
}

static void LabWaitNext(uint32_t *next, uint32_t period)
{
    *next += period;  /* Defined unsigned wrap. */
    const uint32_t now = osKernelGetTickCount();
    const uint32_t remaining = *next - now;
    if (remaining == 0u || remaining > UINT32_C(0x7fffffff)) {
        ++lab03_target_misses;
        *next = now + period;
    }
    osStatus_t status = osDelayUntil(*next);
    if (status == osErrorParameter || status == osError) {
        /*
         * Period is validated. The target can expire between the read
         * and wait call (including the adapter's own tick read).
         * Resynchronise once and wait, never do an immediate extra toggle.
         */
        ++lab03_target_misses;
        *next = osKernelGetTickCount() + period;
        status = osDelayUntil(*next);
    }
    if (status != osOK) {
        LabFail();
    }
}

void Lab03_BindEvent(osThreadId_t event_handle)
{
    if (event_handle == NULL || osKernelGetState() != osKernelReady) {
        LabFail();
    }
    event_thread = event_handle;
}

void Lab03_Heartbeat(void *argument)
{
    (void)argument;
    uint32_t next = osKernelGetTickCount();
    const uint32_t period = LabMsToTicks(500u);
    for (;;) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
        LabWaitNext(&next, period);
    }
}

void Lab03_HeavyMath(void *argument)
{
    (void)argument;
    for (;;) {
        lab03_accumulator =
            lab03_accumulator * 1664525u + 1013904223u;
    }
}

void Lab03_Event(void *argument)
{
    (void)argument;
    if (event_thread == NULL || osKernelGetState() != osKernelRunning) {
        LabFail();
    }
    /*
     * Startup policy: discard button edges pending before this point.
     * No other interrupt source may use this shared IRQ in this lab.
     */
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
    HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    for (;;) {
        const uint32_t flags =
            osThreadFlagsWait(1u, osFlagsWaitAny, osWaitForever);
        if ((flags & osFlagsError) != 0u) {
            LabFail();
        }
        ++lab03_event_responses;
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    }
}

void Lab03_ButtonIRQ(uint16_t gpio_pin)
{
    if (gpio_pin != GPIO_PIN_13) {
        return;
    }
    if (event_thread == NULL) {
        ++lab03_irq_errors;
        return;
    }
    const uint32_t flags = osThreadFlagsSet(event_thread, 1u);
    if ((flags & osFlagsError) != 0u) {
        ++lab03_irq_errors;
    }
}
