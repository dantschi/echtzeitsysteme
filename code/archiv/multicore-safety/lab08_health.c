#include "main.h"
#include "cmsis_os2.h"
#include "lab08_health.h"
#if !defined(LAB08_BUTTON_Pin) || !defined(LAB08_BUTTON_GPIO_Port) || \
    !defined(LAB08_GOOD_Pin) || !defined(LAB08_GOOD_GPIO_Port) || \
    !defined(LAB08_BAD_Pin) || !defined(LAB08_BAD_GPIO_Port) || \
    !defined(LAB08_STATUS_Pin) || !defined(LAB08_STATUS_GPIO_Port)
#error "Configure onboard button and three LED labels"
#endif
static osMutexId_t lock;
static Lab08_Health health;
static int pause_b, started;
volatile Lab08_Health lab08_health_snapshot;
volatile uint32_t lab08_health_valid;
static void Check(osStatus_t status)
{
    if (status != osOK) { Error_Handler(); for (;;) {} }
}
static void Worker(void *argument)
{
    const uint32_t index = *(const uint32_t *)argument;
    for (;;) {
        Check(osMutexAcquire(lock, osWaitForever));
        if (index == 0U) {
            health.last_a = osKernelGetTickCount(); health.have_a = 1;
        } else if (!pause_b) {
            health.last_b = osKernelGetTickCount(); health.have_b = 1;
        }
        Check(osMutexRelease(lock));
        Check(osDelay(index == 0U ? 60U : 100U));
    }
}
static void Supervisor(void *argument)
{
    (void)argument;
    uint32_t rounds = 0U;
    for (;;) {
        const int pressed = HAL_GPIO_ReadPin(LAB08_BUTTON_GPIO_Port,
                                             LAB08_BUTTON_Pin) == GPIO_PIN_SET;
        Check(osMutexAcquire(lock, osWaitForever));
        pause_b = pressed;
        Lab08_Health copy = health;
        copy.now = osKernelGetTickCount();
        Check(osMutexRelease(lock));
        lab08_health_snapshot = copy;
        lab08_health_valid = Lab08_Healthy(&copy) ? 1U : 0U;
        HAL_GPIO_WritePin(LAB08_GOOD_GPIO_Port, LAB08_GOOD_Pin,
                          lab08_health_valid ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LAB08_BAD_GPIO_Port, LAB08_BAD_Pin,
                          lab08_health_valid ? GPIO_PIN_RESET : GPIO_PIN_SET);
        if (++rounds == 10U) {
            HAL_GPIO_TogglePin(LAB08_STATUS_GPIO_Port, LAB08_STATUS_Pin);
            rounds = 0U;
        }
        Check(osDelay(20U));
    }
}
void Lab08_HealthStart(void)
{
    static const uint32_t index[2] = {0U, 1U};
    const osMutexAttr_t mutex_attr = {.attr_bits = osMutexPrioInherit};
    const osThreadAttr_t attr = {
        .name = "HealthWorker", .stack_size = 1024U, .priority = osPriorityNormal
    };
    const osThreadAttr_t supervisor_attr = {
        .name = "Supervisor", .stack_size = 1024U, .priority = osPriorityAboveNormal
    };
    if (started || osKernelGetState() != osKernelReady ||
        osKernelGetTickFreq() != 1000U) { Check(osError); }
    started = 1;
    lock = osMutexNew(&mutex_attr);
    if (lock == NULL || osThreadNew(Worker, (void *)&index[0], &attr) == NULL ||
        osThreadNew(Worker, (void *)&index[1], &attr) == NULL ||
        osThreadNew(Supervisor, NULL, &supervisor_attr) == NULL) { Check(osError); }
}
