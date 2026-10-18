#include "lab02_demo.h"
#include "main.h"

#ifndef LAB_MARKER_GPIO_Port
#error "Configure a free CM4 GPIO with CubeMX user label LAB_MARKER."
#endif
#ifndef LAB_MARKER_Pin
#error "Configure a free CM4 GPIO with CubeMX user label LAB_MARKER."
#endif

_Static_assert(LAB02_SMALL_WORK > 0U, "Small work must be positive.");
_Static_assert(LAB02_LARGE_WORK > LAB02_SMALL_WORK,
               "Large work must exceed small work.");

volatile Lab02_Diagnostics lab02_diag;
static int initialized;
static Lab02_Mode mode;
static uint8_t rr_cursor;
static uint8_t running_idx = LAB02_NONE;

static void TaskA_step(void);
static void TaskB_step(void);
static void TaskC_step(void);

static Lab02_TCB task_list[LAB02_N] = {
    { TaskA_step, ST_READY, 0U },
    { TaskB_step, ST_READY, 0U },
    { TaskC_step, ST_READY, 0U }
};

static void Lab02_Fail(void)
{
    ++lab02_diag.failures;
    Error_Handler();
    for (;;) {}
}

int Lab02_TimeReached(uint32_t now, uint32_t target)
{
    return (uint32_t)(now - target) < LAB02_HALF_RANGE;
}

static int Lab02_InitWithSteps(Lab02_Mode selected,
                              void (*const steps[LAB02_N])(void))
{
    if (__get_IPSR() != 0U || initialized || selected < LAB02_SHORT || selected > LAB02_SLEEP_A ||
        HAL_GetTickFreq() != HAL_TICK_FREQ_1KHZ || steps == NULL) {
        return 0;
    }
    for (uint8_t i = 0U; i < LAB02_N; ++i) {
        if (steps[i] == NULL) {
            return 0;
        }
    }
    mode = selected;
    rr_cursor = 0U;
    running_idx = LAB02_NONE;
    for (uint8_t i = 0U; i < LAB02_N; ++i) {
        task_list[i].step = steps[i];
        task_list[i].state = ST_READY;
        task_list[i].wake_at = 0U;
        lab02_diag.runs[i] = 0U;
        lab02_diag.result[i] = 0U;
    }
    lab02_diag.idle_entries = 0U;
    lab02_diag.failures = 0U;
    HAL_GPIO_WritePin(LAB_MARKER_GPIO_Port, LAB_MARKER_Pin, GPIO_PIN_RESET);
    initialized = 1;
    return 1;
}

int Lab02_Init(Lab02_Mode selected)
{
    void (*const steps[LAB02_N])(void) = {
        TaskA_step, TaskB_step, TaskC_step
    };
    return Lab02_InitWithSteps(selected, steps);
}

int Lab02_InitSteps(void (*const steps[LAB02_N])(void))
{
    return Lab02_InitWithSteps(LAB02_SHORT, steps);
}

int Lab02_SleepAfterStep(uint32_t ms)
{
    if (__get_IPSR() != 0U || !initialized || running_idx >= LAB02_N ||
        ms == 0U || ms >= LAB02_HALF_RANGE) {
        return 0;
    }
    task_list[running_idx].wake_at = HAL_GetTick() + ms;
    task_list[running_idx].state = ST_BLOCKED;
    return 1;
}

static void LabCpuWork(uint32_t iterations, uint8_t index)
{
    while (iterations > 0U) {
        lab02_diag.result[index] =
            lab02_diag.result[index] * UINT32_C(1664525) + UINT32_C(1013904223);
        --iterations;
    }
}

static void TaskA_step(void)
{
    HAL_GPIO_WritePin(LAB_MARKER_GPIO_Port, LAB_MARKER_Pin, GPIO_PIN_SET);
    LabCpuWork(LAB02_SMALL_WORK, 0U);
    HAL_GPIO_WritePin(LAB_MARKER_GPIO_Port, LAB_MARKER_Pin, GPIO_PIN_RESET);
    ++lab02_diag.runs[0];
    if (mode == LAB02_SLEEP_A) {
        if (!Lab02_SleepAfterStep(10U)) {
            Lab02_Fail();
        }
        return;
    }
}

static void TaskB_step(void)
{
    uint32_t work = mode == LAB02_LONG_B ? LAB02_LARGE_WORK : LAB02_SMALL_WORK;
    if (mode == LAB02_VARIABLE_B) {
        work = (lab02_diag.runs[1] & 1U) == 0U ? LAB02_SMALL_WORK : LAB02_LARGE_WORK;
    }
    LabCpuWork(work, 1U);
    ++lab02_diag.runs[1];
}

static void TaskC_step(void)
{
    LabCpuWork(LAB02_SMALL_WORK, 2U);
    ++lab02_diag.runs[2];
}

void Lab02_DispatchOnce(void)
{
    if (__get_IPSR() != 0U || !initialized || running_idx != LAB02_NONE) {
        Lab02_Fail();
    }
    uint32_t now = HAL_GetTick();
    for (uint8_t i = 0U; i < LAB02_N; ++i) {
        if (task_list[i].state != ST_READY && task_list[i].state != ST_BLOCKED) {
            Lab02_Fail();
        }
        if (task_list[i].state == ST_BLOCKED &&
            Lab02_TimeReached(now, task_list[i].wake_at)) {
            task_list[i].state = ST_READY;
        }
    }
    int selected = -1;
    for (uint8_t k = 0U; k < LAB02_N; ++k) {
        uint8_t i = (uint8_t)((rr_cursor + k) % LAB02_N);
        if (task_list[i].state == ST_READY) {
            selected = (int)i;
            break;
        }
    }
    if (selected < 0) {
        ++lab02_diag.idle_entries;
        __WFI(); /* A running HAL tick/wake source is a setup prerequisite. */
        return;
    }
    rr_cursor = (uint8_t)(((unsigned)selected + 1U) % LAB02_N);
    running_idx = (uint8_t)selected;
    task_list[selected].step();
    running_idx = LAB02_NONE;
}
