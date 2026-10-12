/* VL5 application module: CM4, CMSIS-RTOS2/FreeRTOS, NUCLEO-H755ZI-Q.
 * Needs a working generated CubeMX project, dynamic queues/semaphores,
 * preemption, valid stack/heap, and a 32-bit kernel tick at 1000 Hz.
 * Producer Normal; Consumer AboveNormal. Part B runs separately with just
 * ButtonWorker AboveNormal. Disable earlier application loads/IRQs.
 * No UART, GPIO, debounce or microsecond timing is implemented here. */
#include "lab05_messages.h"
#include "main.h"

static osMessageQueueId_t data_queue;
static osMessageQueueId_t button_queue;
static osSemaphoreId_t parking_gate;
static int initialized;

volatile Lab05_Attempt lab05_attempt[LAB05_ATTEMPTS];
volatile Lab05_Received lab05_received[LAB05_ATTEMPTS];
volatile uint32_t lab05_accepted;
volatile uint32_t lab05_dropped;
volatile uint32_t lab05_received_count;
volatile uint32_t lab05_producer_done;
volatile uint32_t lab05_consumer_done;
volatile uint32_t lab05_button_offered;
volatile uint32_t lab05_button_accepted;
volatile uint32_t lab05_button_dropped;
volatile uint32_t lab05_button_errors;
volatile uint32_t lab05_button_processed;
volatile Lab05_ButtonEvent lab05_button_last;
volatile uint32_t lab05_button_last_received_tick;

static void Fail(void)
{
    Error_Handler();
    for (;;) {}
}

static void Check(osStatus_t status)
{
    if (status != osOK) {
        Fail();
    }
}

static void Park(void)
{
    for (;;) {
        Check(osSemaphoreAcquire(parking_gate, osWaitForever));
    }
}

void Lab05_Init(void)
{
    if (initialized || osKernelGetState() != osKernelReady) {
        Fail();
    }
    data_queue = osMessageQueueNew(LAB05_QUEUE_CAPACITY,
                                  sizeof(Lab05_Message), NULL);
    button_queue = osMessageQueueNew(LAB05_QUEUE_CAPACITY,
                                    sizeof(Lab05_ButtonEvent), NULL);
    parking_gate = osSemaphoreNew(1u, 0u, NULL);
    if (data_queue == NULL || button_queue == NULL || parking_gate == NULL) {
        Fail();
    }
    initialized = 1;
}

void Lab05_Producer(void *argument)
{
    (void)argument;
    if (!initialized) {
        Fail();
    }
    for (uint32_t i = 0u; i < LAB05_ATTEMPTS; ++i) {
        Lab05_Message message = {
            .sample = 10u * (i + 1u),
            .seq = i + 1u,
            .tick = osKernelGetTickCount(),
            .kind = LAB05_DATA
        };
        osStatus_t status = osMessageQueuePut(data_queue, &message, 0u, 0u);
        lab05_attempt[i].tick = message.tick;
        lab05_attempt[i].status = status;
        if (status == osOK) {
            ++lab05_accepted;
        } else if (status == osErrorResource) {
            ++lab05_dropped;
        } else {
            Fail();
        }
        Check(osDelay(10u));
    }
    const Lab05_Message stop = {
        .sample = 0u,
        .seq = 0u,
        .tick = osKernelGetTickCount(),
        .kind = LAB05_STOP
    };
    /* Consumer remains active and no mutex is held across this wait. */
    Check(osMessageQueuePut(data_queue, &stop, 0u, osWaitForever));
    lab05_producer_done = 1u;
    Park();
}

void Lab05_Consumer(void *argument)
{
    (void)argument;
    uint32_t previous_seq = 0u;
    if (!initialized) {
        Fail();
    }
    for (;;) {
        Lab05_Message message;
        Check(osMessageQueueGet(data_queue, &message, NULL, osWaitForever));
        if (message.kind == LAB05_STOP) {
            lab05_consumer_done = 1u;
            Park();
        }
        uint32_t index = lab05_received_count;
        if (message.kind != LAB05_DATA || index >= LAB05_ATTEMPTS ||
            message.seq <= previous_seq || message.seq > LAB05_ATTEMPTS ||
            message.sample != 10u * message.seq) {
            Fail();
        }
        lab05_received[index].message = message;
        lab05_received[index].received_tick = osKernelGetTickCount();
        previous_seq = message.seq;
        lab05_received_count = index + 1u;
        Check(osDelay(40u));
    }
}

void Lab05_ButtonIRQ(void)
{
    /* EXTI stays disabled until Init has completed and kernel is running.
     * Do not call Error_Handler from ISR: it may contain blocking work. */
    if (!initialized || osKernelGetState() != osKernelRunning) {
        ++lab05_button_errors;
        return;
    }
    Lab05_ButtonEvent event = {
        .seq = ++lab05_button_offered,
        .tick = osKernelGetTickCount()
    };
    osStatus_t status = osMessageQueuePut(button_queue, &event, 0u, 0u);
    if (status == osOK) {
        ++lab05_button_accepted;
    } else if (status == osErrorResource) {
        ++lab05_button_dropped;
    } else {
        ++lab05_button_errors;
    }
}

void Lab05_ButtonWorker(void *argument)
{
    (void)argument;
    if (!initialized) {
        Fail();
    }
    for (;;) {
        Lab05_ButtonEvent event;
        Check(osMessageQueueGet(button_queue, &event, NULL, osWaitForever));
        lab05_button_last = event;
        lab05_button_last_received_tick = osKernelGetTickCount();
        ++lab05_button_processed;
        /* Deliberately limited service; not a debounce algorithm. */
        Check(osDelay(50u));
    }
}
