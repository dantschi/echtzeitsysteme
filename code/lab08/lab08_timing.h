#ifndef LAB08_TIMING_H
#define LAB08_TIMING_H

#include <stdint.h>

#if !defined(CORE_CM7) || defined(CORE_CM4)
#error "lab08_timing is only for the privileged CM7 target"
#endif

#define LAB08_CAPACITY 16u
#ifndef LAB08_SAMPLES_PER_KIND
#define LAB08_SAMPLES_PER_KIND 7u
#endif
#ifndef LAB08_WORK_STEPS
#define LAB08_WORK_STEPS UINT32_C(10000)
#endif
#ifndef LAB08_WORK_SEED
#define LAB08_WORK_SEED UINT32_C(7)
#endif

typedef enum {
    LAB08_EMPTY = 1,
    LAB08_WORK = 2
} Lab08_Kind;

typedef enum {
    LAB08_OK = 0,
    LAB08_SETUP = 1,
    LAB08_NO_COUNTER = 2,
    LAB08_COUNTER_STOPPED = 3,
    LAB08_CLOCK_CHANGED = 4,
    LAB08_FULL = 5,
    LAB08_ARGUMENT = 6
} Lab08_Status;

typedef struct {
    uint32_t sequence;
    uint32_t kind;
    uint32_t cycles_mod32;
    uint32_t core_hz;
    uint32_t digest;
} Lab08_Record;

/* Private CM7 diagnostics, outside .shared. Exactly one main-context writer.
 * Read only after logging stops, using the actual image/cache-aware debugger.
 * cycles_mod32 is always a modulo difference. Conversion to elapsed time
 * additionally requires a stable clock and an interval shorter than one wrap.
 * Halt/sleep, clock changes and missed counter periods are not repaired here. */
extern volatile Lab08_Record lab08_records[LAB08_CAPACITY];
extern volatile uint32_t lab08_count;
extern volatile uint32_t lab08_dropped;
extern volatile uint32_t lab08_done;
extern volatile Lab08_Status lab08_status;

/* After verified clock setup; core_hz must match the updated SystemCoreClock.
 * Does not reset CYCCNT or change DWT comparator/ITM/ETM configuration.
 * Reserve cycle-counter ownership with any connected debug/trace tool. */
Lab08_Status Lab08_Init(uint32_t core_hz);
/* One bounded record attempt, no overwrite and no output/heap/kernel calls.
 * Do not call from an ISR or a second thread. */
Lab08_Status Lab08_RecordOne(Lab08_Kind kind);
/* Exactly once: configured empty windows, then synthetic work windows. */
Lab08_Status Lab08_Run(void);

#endif
