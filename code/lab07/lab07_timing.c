/* CM7 counter experiment, not a complete STM32 firmware project.
 * Clock, boot, MPU/cache, time-window assumptions and debugger view are
 * verified in the CubeMX/VS Code project. No claimed board measurements. */
#include "main.h"
#include "lab07_timing.h"

#define DWT_LOCK_PRESENT UINT32_C(1)
#define DWT_LOCKED UINT32_C(2)
#define DWT_UNLOCK_KEY UINT32_C(0xC5ACCE55)

volatile Lab07_Record lab07_records[LAB07_CAPACITY];
volatile uint32_t lab07_count;
volatile uint32_t lab07_dropped;
volatile uint32_t lab07_done;
volatile Lab07_Status lab07_status;
static uint32_t configured_hz;
static int initialized;
static int run_started;

static Lab07_Status Status(Lab07_Status status)
{
    lab07_status = status;
    return status;
}

static Lab07_Status CounterStatus(void)
{
    if (!initialized) {
        return LAB07_SETUP;
    }
    if (SystemCoreClock != configured_hz) {
        return LAB07_CLOCK_CHANGED;
    }
    if ((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) == 0u ||
        (DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) == 0u) {
        return LAB07_COUNTER_STOPPED;
    }
    return LAB07_OK;
}

Lab07_Status Lab07_Init(uint32_t core_hz)
{
    if (initialized || core_hz == 0u || SystemCoreClock != core_hz) {
        return Status(LAB07_SETUP);
    }
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    /* Cortex-M7 LSR: bit 0 = software lock implemented, bit 1 = locked.
     * LAR/LSR are present in the checked CMSIS-M7 DWT register type. */
    if ((DWT->LSR & (DWT_LOCK_PRESENT | DWT_LOCKED)) ==
        (DWT_LOCK_PRESENT | DWT_LOCKED)) {
        DWT->LAR = DWT_UNLOCK_KEY;
    }
    if ((DWT->CTRL & DWT_CTRL_NOCYCCNT_Msk) != 0u) {
        return Status(LAB07_NO_COUNTER);
    }
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    __DSB();
    __ISB();
    uint32_t before = DWT->CYCCNT;
    for (uint32_t i = 0u; i < 128u; ++i) {
        __NOP();
    }
    uint32_t after = DWT->CYCCNT;
    if (after == before) {
        return Status(LAB07_COUNTER_STOPPED);
    }
    configured_hz = core_hz;
    initialized = 1;
    return Status(LAB07_OK);
}

/* Deliberately observable private synthetic workload, not sensor processing.
 * Each volatile access is part of what we measure. The digest is also used.
 * unsigned 32-bit arithmetic gives defined modulo wrap, including shifts. */
static __attribute__((noinline)) uint32_t Work(void)
{
    volatile uint32_t value = LAB07_WORK_SEED;
    for (uint32_t i = 0u; i < LAB07_WORK_STEPS; ++i) {
        uint32_t old = value;
        value = ((old << 5u) | (old >> 27u)) + UINT32_C(0x9E3779B9);
    }
    return value;
}

Lab07_Status Lab07_RecordOne(Lab07_Kind kind)
{
    Lab07_Status status = CounterStatus();
    if (status != LAB07_OK) {
        return Status(status);
    }
    if (kind != LAB07_EMPTY && kind != LAB07_WORK) {
        return Status(LAB07_ARGUMENT);
    }
    if (lab07_count >= LAB07_CAPACITY) {
        ++lab07_dropped;
        return Status(LAB07_FULL);
    }
    uint32_t digest = 0u;
    __DSB();
    __ISB();
    uint32_t begin = DWT->CYCCNT;
    __DMB();
    if (kind == LAB07_WORK) {
        digest = Work();
    }
    __DSB();
    uint32_t end = DWT->CYCCNT;
    status = CounterStatus();
    if (status != LAB07_OK) {
        return Status(status);
    }
    /* Store the record AFTER the timed window. This is not a trace event
     * timestamp and does not certify the single-wrap timing precondition. */
    uint32_t slot = lab07_count;
    lab07_records[slot].sequence = slot + 1u;
    lab07_records[slot].kind = (uint32_t)kind;
    lab07_records[slot].cycles_mod32 = (uint32_t)(end - begin);
    lab07_records[slot].core_hz = configured_hz;
    lab07_records[slot].digest = digest;
    __DMB();
    lab07_count = slot + 1u;
    return Status(LAB07_OK);
}

Lab07_Status Lab07_Run(void)
{
    if (run_started || lab07_count != 0u || CounterStatus() != LAB07_OK) {
        return Status(LAB07_SETUP);
    }
    run_started = 1;
    for (uint32_t kind = LAB07_EMPTY; kind <= LAB07_WORK; ++kind) {
        for (uint32_t i = 0u; i < LAB07_SAMPLES_PER_KIND; ++i) {
            Lab07_Status status = Lab07_RecordOne((Lab07_Kind)kind);
            if (status != LAB07_OK) {
                return status;
            }
        }
    }
    lab07_done = 1u;
    return Status(LAB07_OK);
}
