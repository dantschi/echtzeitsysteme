#include "main.h"
#include "cmsis_os2.h"
#include "lab09_capstone.h"
#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "CM4 source only"
#endif
volatile Lab09_Error lab09_error;
volatile uint32_t lab09_complete;
volatile Lab09_Stats lab09_stats;
volatile Lab09_Record lab09_publications[LAB09_MAX_EVENTS];
volatile uint32_t lab09_publication_count;
static osMessageQueueId_t queue;
static volatile uint32_t accepting;
static uint32_t limit;
static int started;
static void Fail(Lab09_Error error)
{
    lab09_error = error;
    Error_Handler();
    for (;;) {}
}
static int Take(void)
{
    const uint32_t begin = osKernelGetTickCount();
    for (;;) {
        if (HAL_HSEM_Take(LAB09_HSEM_ID, 0x34u) == HAL_OK) { __DMB(); return 1; }
        if ((uint32_t)(osKernelGetTickCount() - begin) >= 20u) { return 0; }
        if (osDelay(1u) != osOK) { Fail(LAB09_KERNEL); }
    }
}
static void Release(void)
{
    __DMB(); HAL_HSEM_Release(LAB09_HSEM_ID, 0x34u);
}
static void Check(void)
{
    if (!Lab09_BoxValid()) { Release(); Fail(LAB09_PROTOCOL); }
}
void Lab09_CM4_OnEvent(uint32_t sample)
{
    if (accepting == 0u || lab09_stats.inputs >= limit) { return; }
    Lab09_Record event = {0};
    event.event_tick = HAL_GetTick(); /* CM4-local HAL time, NOT CM7 time. */
    event.event_seq = ++lab09_stats.inputs;
    event.sample = sample;
    osStatus_t status = osMessageQueuePut(queue, &event, 0u, 0u);
    if (status == osOK) { ++lab09_stats.accepted; }
    else if (status == osErrorResource) { ++lab09_stats.queue_drops; }
    else { ++lab09_stats.put_errors; }
}
static void Publish(Lab09_Record event)
{
    if (event.sample > LAB09_MAX_SAMPLE || !Take()) {
        ++lab09_stats.task_drops; return;
    }
    Check();
    if (lab09_box.pub_seq >= LAB09_MAX_EVENTS) { Release(); Fail(LAB09_PROTOCOL); }
    if (lab09_box.valid != 0u && lab09_box.ack_pub != lab09_box.pub_seq) {
        ++lab09_box.overwritten;
    }
    event.pub_seq = lab09_box.pub_seq + 1u;
    lab09_box.event_seq = event.event_seq;
    lab09_box.pub_seq = event.pub_seq;
    lab09_box.event_tick = event.event_tick;
    lab09_box.payload = event.sample;
    lab09_box.valid = 1u;
    Release();
    ++lab09_stats.published;
    if (lab09_publication_count >= LAB09_MAX_EVENTS) { Fail(LAB09_LOG_FULL); }
    lab09_publications[lab09_publication_count] = event;
    ++lab09_publication_count;
}
void Lab09_CM4_Run(void *argument)
{
    (void)argument;
    if (started || osKernelGetState() != osKernelRunning ||
        osKernelGetTickFreq() != 1000u) { Fail(LAB09_SETUP); }
    started = 1;
    if (!Take()) { Fail(LAB09_LOCK_TIMEOUT); }
    Check();
    const uint32_t mode = lab09_box.mode;
    Release();
    limit = mode == LAB09_SATURATION ? 10u : LAB09_MAX_EVENTS;
    queue = osMessageQueueNew(8u, sizeof(Lab09_Record), NULL);
    if (queue == NULL) { Fail(LAB09_KERNEL); }
    __DMB(); accepting = 1u;
    if (mode == LAB09_SATURATION) {
        /* Exact capacity test: ten correct Puts BEFORE any Get, from task.
         * This does not claim ten physical timer edges or test ISR priority. */
        for (uint32_t i = 1u; i <= 10u; ++i) { Lab09_CM4_OnEvent(i * 10u); }
    } else if (!Lab09_CM4_StartEvents()) { Fail(LAB09_SOURCE); }
    const uint32_t begin = osKernelGetTickCount();
    for (;;) {
        Lab09_Record event;
        osStatus_t status = osMessageQueueGet(queue, &event, NULL, 1u);
        if (status == osOK) {
            lab09_stats.in_flight = 1u;
            Publish(event);
            lab09_stats.in_flight = 0u;
        } else if (status != osErrorTimeout && status != osErrorResource) {
            Fail(LAB09_KERNEL);
        }
        if ((uint32_t)(osKernelGetTickCount() - begin) >= 2000u) {
            Fail(LAB09_SOURCE);
        }
        if (lab09_stats.inputs >= limit && osMessageQueueGetCount(queue) == 0u) {
            break;
        }
    }
    accepting = 0u; __DMB();
    if (mode == LAB09_NORMAL && !Lab09_CM4_StopEvents()) { Fail(LAB09_SOURCE); }
    lab09_stats.queued = osMessageQueueGetCount(queue);
    if (osMessageQueueDelete(queue) != osOK) { Fail(LAB09_KERNEL); }
    queue = NULL;
    if (!Take()) { Fail(LAB09_LOCK_TIMEOUT); }
    Check();
    lab09_box.producer_done = 1u;
    Release();
    lab09_complete = 1u;
    osThreadExit();
    Fail(LAB09_KERNEL);
}
