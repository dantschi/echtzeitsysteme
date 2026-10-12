/*
 * VL4 teaching module, NUCLEO-H755ZI-Q, CM4, CMSIS-RTOS2/FreeRTOS.
 * Requires a working generated dual-core project, preemption enabled,
 * L=osPriorityNormal, M=osPriorityAboveNormal, H=osPriorityHigh.
 * Disable other application loads and kernel-related button callbacks.
 * Reboot for each variant; use identical work, build and clock settings.
 * Tick timestamps are wall time, not exclusive thread CPU time.
 */
#include "lab04_resources.h"
#include "main.h"

#define LOW_ITERATIONS    UINT32_C(200000)
#define MEDIUM_ITERATIONS UINT32_C(800000)
#define HIGH_ITERATIONS   UINT32_C(1000)

static osSemaphoreId_t high_gate;
static osSemaphoreId_t medium_gate;
static osSemaphoreId_t parking_gate;
static osSemaphoreId_t test_semaphore;
static osMutexId_t test_mutex;
static Lab04_Mode test_mode;
static int initialized;

volatile Lab04_Log lab04_log[3];
volatile uint32_t lab04_result[3];
volatile osPriority_t lab04_low_effective_priority = osPriorityError;

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

static void CpuWork(uint32_t n, volatile uint32_t *sink)
{
    while (n-- > 0u) {
        *sink = *sink * 1664525u + 1013904223u;
    }
}

static osStatus_t TestAcquire(void)
{
    if (test_mode == LAB04_MUTEX) {
        return osMutexAcquire(test_mutex, osWaitForever);
    }
    return osSemaphoreAcquire(test_semaphore, osWaitForever);
}

static osStatus_t TestRelease(void)
{
    if (test_mode == LAB04_MUTEX) {
        return osMutexRelease(test_mutex);
    }
    return osSemaphoreRelease(test_semaphore);
}

static void Park(void)
{
    /* No thread in this module releases this empty semaphore. */
    for (;;) {
        Check(osSemaphoreAcquire(parking_gate, osWaitForever));
    }
}

void Lab04_Init(Lab04_Mode mode)
{
    if (initialized || osKernelGetState() != osKernelReady ||
        (mode != LAB04_SEMAPHORE && mode != LAB04_MUTEX)) {
        Fail();
    }
    test_mode = mode;
    high_gate = osSemaphoreNew(1u, 0u, NULL);
    medium_gate = osSemaphoreNew(1u, 0u, NULL);
    parking_gate = osSemaphoreNew(1u, 0u, NULL);
    if (high_gate == NULL || medium_gate == NULL || parking_gate == NULL) {
        Fail();
    }
    if (mode == LAB04_MUTEX) {
        const osMutexAttr_t attr = {
            .name = "InversionTest",
            .attr_bits = osMutexPrioInherit
        };
        test_mutex = osMutexNew(&attr);
        if (test_mutex == NULL) {
            Fail();
        }
    } else {
        test_semaphore = osSemaphoreNew(1u, 1u, NULL);
        if (test_semaphore == NULL) {
            Fail();
        }
    }
    initialized = 1;
}

void Lab04_Low(void *argument)
{
    (void)argument;
    if (!initialized) {
        Fail();
    }
    lab04_log[LAB04_LOW].started_tick = osKernelGetTickCount();
    lab04_log[LAB04_LOW].stage = LAB04_LOCK_REQUEST;
    lab04_log[LAB04_LOW].request_tick = osKernelGetTickCount();
    Check(TestAcquire());
    lab04_log[LAB04_LOW].acquired_tick = osKernelGetTickCount();
    lab04_log[LAB04_LOW].stage = LAB04_LOCK_ACQUIRED;

    /* H was blocked at its gate. Opening it lets H run and request the lock. */
    lab04_log[LAB04_LOW].gate_release_request_tick = osKernelGetTickCount();
    Check(osSemaphoreRelease(high_gate));
    lab04_low_effective_priority = osThreadGetPriority(osThreadGetId());
    if (lab04_low_effective_priority == osPriorityError) {
        Fail();
    }
    lab04_log[LAB04_LOW].stage = LAB04_WORK;
    CpuWork(LOW_ITERATIONS, &lab04_result[LAB04_LOW]);
    /* This is BEFORE the release call, not a confirmed release timestamp. */
    lab04_log[LAB04_LOW].release_request_tick = osKernelGetTickCount();
    Check(TestRelease());
    lab04_log[LAB04_LOW].done_tick = osKernelGetTickCount();
    lab04_log[LAB04_LOW].stage = LAB04_FINISHED;
    Park();
}

void Lab04_Medium(void *argument)
{
    (void)argument;
    if (!initialized) {
        Fail();
    }
    lab04_log[LAB04_MEDIUM].stage = LAB04_GATE_WAIT;
    Check(osSemaphoreAcquire(medium_gate, osWaitForever));
    lab04_log[LAB04_MEDIUM].started_tick = osKernelGetTickCount();
    lab04_log[LAB04_MEDIUM].stage = LAB04_WORK;
    CpuWork(MEDIUM_ITERATIONS, &lab04_result[LAB04_MEDIUM]);
    lab04_log[LAB04_MEDIUM].done_tick = osKernelGetTickCount();
    lab04_log[LAB04_MEDIUM].stage = LAB04_FINISHED;
    Park();
}

void Lab04_High(void *argument)
{
    (void)argument;
    if (!initialized) {
        Fail();
    }
    lab04_log[LAB04_HIGH].stage = LAB04_GATE_WAIT;
    Check(osSemaphoreAcquire(high_gate, osWaitForever));
    lab04_log[LAB04_HIGH].started_tick = osKernelGetTickCount();
    /*
     * Make M ready before blocking on L's lock. H is still higher than M.
     * Without inheritance M then wins. With inheritance boosted L wins.
     */
    lab04_log[LAB04_HIGH].gate_release_request_tick = osKernelGetTickCount();
    Check(osSemaphoreRelease(medium_gate));
    lab04_log[LAB04_HIGH].stage = LAB04_LOCK_REQUEST;
    lab04_log[LAB04_HIGH].request_tick = osKernelGetTickCount();
    Check(TestAcquire());
    lab04_log[LAB04_HIGH].acquired_tick = osKernelGetTickCount();
    lab04_log[LAB04_HIGH].stage = LAB04_LOCK_ACQUIRED;
    CpuWork(HIGH_ITERATIONS, &lab04_result[LAB04_HIGH]);
    lab04_log[LAB04_HIGH].release_request_tick = osKernelGetTickCount();
    Check(TestRelease());
    lab04_log[LAB04_HIGH].done_tick = osKernelGetTickCount();
    lab04_log[LAB04_HIGH].stage = LAB04_FINISHED;
    Park();
}
