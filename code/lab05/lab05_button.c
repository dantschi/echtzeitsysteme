#include "main.h"
#include "lab05_button.h"
#if !defined(LAB05_BUTTON_Pin) || !defined(LAB05_BUTTON_GPIO_Port) || \
    !defined(LAB05_LED_Pin) || !defined(LAB05_LED_GPIO_Port)
#error "Configure onboard button and LED labels"
#endif
osMessageQueueId_t lab05_press_queue;
volatile uint32_t lab05_irq_offered, lab05_irq_accepted;
volatile uint32_t lab05_irq_dropped, lab05_irq_errors;
volatile uint32_t lab05_worker_handled, lab05_worker_ignored, lab05_worker_pulses;
static int started;
void Lab05_ButtonCheck(osStatus_t status)
{
    if (status != osOK) { Error_Handler(); for (;;) {} }
}
void Lab05_ButtonLed(int on)
{
    HAL_GPIO_WritePin(LAB05_LED_GPIO_Port, LAB05_LED_Pin,
                      on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
static void Worker(void *argument)
{
    __HAL_GPIO_EXTI_CLEAR_IT(LAB05_BUTTON_Pin);
    HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    Lab05_PulseWorker(argument);
    Lab05_ButtonCheck(osError); /* Thread must not return. */
}
void Lab05_ButtonStart(void)
{
    const osThreadAttr_t attr = {
        .name = "ButtonPulse", .stack_size = 1024U, .priority = osPriorityAboveNormal
    };
    if (started || osKernelGetState() != osKernelReady ||
        osKernelGetTickFreq() != 1000U) { Lab05_ButtonCheck(osError); }
    started = 1;
    Lab05_ButtonLed(0);
    lab05_press_queue = osMessageQueueNew(3U, sizeof(Lab05_Press), NULL);
    if (lab05_press_queue == NULL || osThreadNew(Worker, NULL, &attr) == NULL) {
        Lab05_ButtonCheck(osError);
    }
}
void Lab05_ButtonIRQ(uint16_t pin)
{
    if (pin != LAB05_BUTTON_Pin) { return; }
    if (!started || osKernelGetState() != osKernelRunning) {
        ++lab05_irq_errors; return;
    }
    Lab05_QueueButton();
}
