/* Prepared support for lab 3. Student code supplies both thread bodies.
   This module is independent of the earlier lab03_rtos demonstration.
   CubeMX/board project supplies startup, kernel, HAL timebase and GPIO. */
#include "lab03_tank.h"
#include "main.h"

#if !defined(TANK_LED_GPIO_Port) || !defined(TANK_LED_Pin)
#error "Configure a CM4 LED output labelled TANK_LED."
#endif

osThreadId_t lab03_tank_alarm_handle;
volatile uint32_t lab03_tank_level;
volatile uint32_t lab03_tank_requests;
volatile uint32_t lab03_tank_responses;
volatile uint32_t lab03_tank_target_misses;
static int started;
static uint32_t sample_index;

_Noreturn void Lab03_TankFail(void)
{
    Error_Handler();
    for (;;) {}
}

void Lab03_TankStart(void)
{
    static const osThreadAttr_t alarm_attr = {
        .name = "TankAlarm",
        .stack_size = 1024U,
        .priority = osPriorityAboveNormal
    };
    static const osThreadAttr_t sampler_attr = {
        .name = "TankSampler",
        .stack_size = 1024U,
        .priority = osPriorityNormal
    };
    if (started || osKernelGetState() != osKernelReady ||
        osKernelGetTickFreq() != 1000U) {
        Lab03_TankFail();
    }
    started = 1;
    HAL_GPIO_WritePin(TANK_LED_GPIO_Port, TANK_LED_Pin, GPIO_PIN_RESET);
    lab03_tank_alarm_handle = osThreadNew(Lab03_TankAlarm, NULL, &alarm_attr);
    if (lab03_tank_alarm_handle == NULL ||
        osThreadNew(Lab03_TankSampler, NULL, &sampler_attr) == NULL) {
        Lab03_TankFail();
    }
}

uint32_t Lab03_TankRead(void)
{
    static const uint32_t samples[] = {25U, 45U, 85U, 90U, 55U, 30U};
    lab03_tank_level = samples[sample_index];
    sample_index = (sample_index + 1U) % 6U;
    return lab03_tank_level;
}

void Lab03_TankLed(int on)
{
    HAL_GPIO_WritePin(TANK_LED_GPIO_Port, TANK_LED_Pin,
                      on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Lab03_TankWaitNext(uint32_t *next)
{
    if (next == NULL) {
        Lab03_TankFail();
    }
    *next += LAB03_TANK_SAMPLE_TICKS;
    const uint32_t remaining = *next - osKernelGetTickCount();
    if (remaining == 0U || remaining > UINT32_C(0x7fffffff)) {
        ++lab03_tank_target_misses;
        *next = osKernelGetTickCount() + LAB03_TANK_SAMPLE_TICKS;
    }
    osStatus_t status = osDelayUntil(*next);
    if (status == osErrorParameter || status == osError) {
        /* Target may expire between the read and the adapter's tick read.
           Retry once with a new future target, never a catch-up burst. */
        ++lab03_tank_target_misses;
        *next = osKernelGetTickCount() + LAB03_TANK_SAMPLE_TICKS;
        status = osDelayUntil(*next);
    }
    if (status != osOK) {
        Lab03_TankFail();
    }
}
