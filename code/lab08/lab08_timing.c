/* CM7 counter experiment, not a complete STM32 firmware project.
 * Clock, boot, MPU/cache, time-window assumptions and debugger view are
 * verified in the CubeMX/VS Code project. No claimed board measurements. */
#include "main.h"
#include "lab08_timing.h"

#define DWT_LOCK_PRESENT UINT32_C(1)
#define DWT_LOCKED UINT32_C(2)
#define DWT_UNLOCK_KEY UINT32_C(0xC5ACCE55)

_Static_assert(LAB08_SAMPLES_PER_KIND > 0U &&
               2U * LAB08_SAMPLES_PER_KIND <= LAB08_CAPACITY,
               "Both groups must fit the fixed log");
volatile Lab08_Record lab08_records[LAB08_CAPACITY];
volatile uint32_t lab08_count;
volatile uint32_t lab08_dropped;
volatile uint32_t lab08_done;
volatile Lab08_Status lab08_status;
static uint32_t configured_hz;
static int initialized;
static int run_started;

static Lab08_Status Status(Lab08_Status status)
{
    lab08_status = status;
    return status;
}

static Lab08_Status CounterStatus(void)
{
    if (!initialized) {
        return LAB08_SETUP;
    }
    if (SystemCoreClock != configured_hz) {
        return LAB08_CLOCK_CHANGED;
    }
    if ((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) == 0u ||
        (DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) == 0u) {
        return LAB08_COUNTER_STOPPED;
    }
    return LAB08_OK;
}

Lab08_Status Lab08_Init(uint32_t core_hz)
{
    if (initialized || core_hz == 0u || SystemCoreClock != core_hz) {
        return Status(LAB08_SETUP);
    }
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    /* Cortex-M7 LSR: bit 0 = software lock implemented, bit 1 = locked.
     * LAR/LSR are present in the checked CMSIS-M7 DWT register type. */
    if ((DWT->LSR & (DWT_LOCK_PRESENT | DWT_LOCKED)) ==
        (DWT_LOCK_PRESENT | DWT_LOCKED)) {
        DWT->LAR = DWT_UNLOCK_KEY;
    }
    if ((DWT->CTRL & DWT_CTRL_NOCYCCNT_Msk) != 0u) {
        return Status(LAB08_NO_COUNTER);
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
        return Status(LAB08_COUNTER_STOPPED);
    }
    configured_hz = core_hz;
    initialized = 1;
    return Status(LAB08_OK);
}

/* Deliberately observable private synthetic workload, not sensor processing.
 * Each volatile access is part of what we measure. The digest is also used.
 * unsigned 32-bit arithmetic gives defined modulo wrap, including shifts. */
static __attribute__((noinline)) uint32_t Work(void)
{
    volatile uint32_t value = LAB08_WORK_SEED;
    for (uint32_t i = 0u; i < LAB08_WORK_STEPS; ++i) {
        uint32_t old = value;
        value = ((old << 5u) | (old >> 27u)) + UINT32_C(0x9E3779B9);
    }
    return value;
}

Lab08_Status Lab08_RecordOne(Lab08_Kind kind)
{
    Lab08_Status status = CounterStatus();
    if (status != LAB08_OK) {
        return Status(status);
    }
    if (kind != LAB08_EMPTY && kind != LAB08_WORK) {
        return Status(LAB08_ARGUMENT);
    }
    if (lab08_count >= LAB08_CAPACITY) {
        ++lab08_dropped;
        return Status(LAB08_FULL);
    }
    uint32_t digest = 0u;
    __DSB();
    __ISB();
    uint32_t begin = DWT->CYCCNT;
    __DMB();
    if (kind == LAB08_WORK) {
        digest = Work();
    }
    __DSB();
    uint32_t end = DWT->CYCCNT;
    status = CounterStatus();
    if (status != LAB08_OK) {
        return Status(status);
    }
    /* Store the record AFTER the timed window. This is not a trace event
     * timestamp and does not certify the single-wrap timing precondition. */
    uint32_t slot = lab08_count;
    lab08_records[slot].sequence = slot + 1u;
    lab08_records[slot].kind = (uint32_t)kind;
    lab08_records[slot].cycles_mod32 = (uint32_t)(end - begin);
    lab08_records[slot].core_hz = configured_hz;
    lab08_records[slot].digest = digest;
    __DMB();
    lab08_count = slot + 1u;
    return Status(LAB08_OK);
}

Lab08_Status Lab08_Run(void)
{
    if (run_started || lab08_count != 0u || CounterStatus() != LAB08_OK) {
        return Status(LAB08_SETUP);
    }
    run_started = 1;
    for (uint32_t kind = LAB08_EMPTY; kind <= LAB08_WORK; ++kind) {
        for (uint32_t i = 0u; i < LAB08_SAMPLES_PER_KIND; ++i) {
            Lab08_Status status = Lab08_RecordOne((Lab08_Kind)kind);
            if (status != LAB08_OK) {
                return status;
            }
        }
    }
    lab08_done = 1u;
    return Status(LAB08_OK);
}
