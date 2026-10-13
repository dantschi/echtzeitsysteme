#ifndef LAB07_TIMING_H
#define LAB07_TIMING_H

#include <stdint.h>

#if !defined(CORE_CM7) || defined(CORE_CM4)
#error "lab07_timing is only for the privileged CM7 target"
#endif

#define LAB07_CAPACITY 16u
#define LAB07_SAMPLES_PER_KIND 7u
#define LAB07_WORK_STEPS UINT32_C(10000)
#define LAB07_WORK_SEED UINT32_C(7)

typedef enum {
    LAB07_EMPTY = 1,
    LAB07_WORK = 2
} Lab07_Kind;

typedef enum {
    LAB07_OK = 0,
    LAB07_SETUP = 1,
    LAB07_NO_COUNTER = 2,
    LAB07_COUNTER_STOPPED = 3,
    LAB07_CLOCK_CHANGED = 4,
    LAB07_FULL = 5,
    LAB07_ARGUMENT = 6
} Lab07_Status;

typedef struct {
    uint32_t sequence;
    uint32_t kind;
    uint32_t cycles_mod32;
    uint32_t core_hz;
    uint32_t digest;
} Lab07_Record;

/* Private CM7 diagnostics, outside .shared. Exactly one main-context writer.
 * Read only after logging stops, using the actual image/cache-aware debugger.
 * cycles_mod32 is always a modulo difference. Conversion to elapsed time
 * additionally requires a stable clock and an interval shorter than one wrap.
 * Halt/sleep, clock changes and missed counter periods are not repaired here. */
extern volatile Lab07_Record lab07_records[LAB07_CAPACITY];
extern volatile uint32_t lab07_count;
extern volatile uint32_t lab07_dropped;
extern volatile uint32_t lab07_done;
extern volatile Lab07_Status lab07_status;

/* After verified clock setup; core_hz must match the updated SystemCoreClock.
 * Does not reset CYCCNT or change DWT comparator/ITM/ETM configuration.
 * Reserve cycle-counter ownership with any connected debug/trace tool. */
Lab07_Status Lab07_Init(uint32_t core_hz);
/* One bounded record attempt, no overwrite and no output/heap/kernel calls.
 * Do not call from an ISR or a second thread. */
Lab07_Status Lab07_RecordOne(Lab07_Kind kind);
/* Exactly once: seven empty windows, then seven synthetic work windows. */
Lab07_Status Lab07_Run(void);

#endif
