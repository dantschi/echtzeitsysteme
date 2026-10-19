#include "main.h"
#include "lab04_account.h"
#if !defined(LAB04_GOOD_Pin) || !defined(LAB04_GOOD_GPIO_Port) || \
    !defined(LAB04_BAD_Pin) || !defined(LAB04_BAD_GPIO_Port) || \
    !defined(LAB04_STATUS_Pin) || !defined(LAB04_STATUS_GPIO_Port)
#error "Configure all three onboard LED labels"
#endif
osMutexId_t lab04_account_mutex;
Lab04_Account lab04_account = {1000, 2001, 0U};
static int started;
void Lab04_Check(osStatus_t status)
{
    if (status != osOK) { Error_Handler(); for (;;) {} }
}
static void Writer(void *argument)
{
    const int32_t delta = *(const int32_t *)argument;
    for (uint32_t i = 0U; i < 100U; ++i) {
        Lab04_Adjust(delta);
        Lab04_Check(osDelay(delta > 0 ? 2U : 3U));
    }
    Lab04_Check(osMutexAcquire(lab04_account_mutex, osWaitForever));
    ++lab04_account.done;
    Lab04_Check(osMutexRelease(lab04_account_mutex));
    for (;;) { Lab04_Check(osDelay(1000U)); }
}
static void Reader(void *argument)
{
    (void)argument;
    for (;;) {
        const Lab04_Account copy = Lab04_Snapshot();
        const int valid = copy.check == 2 * copy.balance + 1;
        const int bad_end = copy.done == 2U && copy.balance != 1100;
        HAL_GPIO_WritePin(LAB04_GOOD_GPIO_Port, LAB04_GOOD_Pin,
                          valid ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LAB04_BAD_GPIO_Port, LAB04_BAD_Pin,
                          !valid || bad_end ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_TogglePin(LAB04_STATUS_GPIO_Port, LAB04_STATUS_Pin);
        Lab04_Check(osDelay(100U));
    }
}
void Lab04_AccountStart(void)
{
    static const int32_t deltas[] = {3, -2};
    const osMutexAttr_t mutex_attr = {.attr_bits = osMutexPrioInherit};
    const osThreadAttr_t writer_attr = {
        .name = "AccountWriter", .stack_size = 1024U, .priority = osPriorityNormal
    };
    const osThreadAttr_t reader_attr = {
        .name = "AccountReader", .stack_size = 1024U, .priority = osPriorityAboveNormal
    };
    if (started || osKernelGetState() != osKernelReady ||
        osKernelGetTickFreq() != 1000U) { Lab04_Check(osError); }
    started = 1;
    lab04_account_mutex = osMutexNew(&mutex_attr);
    if (lab04_account_mutex == NULL ||
        osThreadNew(Writer, (void *)&deltas[0], &writer_attr) == NULL ||
        osThreadNew(Writer, (void *)&deltas[1], &writer_attr) == NULL ||
        osThreadNew(Reader, NULL, &reader_attr) == NULL) { Lab04_Check(osError); }
}
