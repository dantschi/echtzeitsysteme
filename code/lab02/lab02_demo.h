#ifndef LAB02_DEMO_H
#define LAB02_DEMO_H

#include <stdint.h>

#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "Build lab02_demo only for the Cortex-M4 application."
#endif

#ifndef LAB02_SMALL_WORK
#define LAB02_SMALL_WORK UINT32_C(30000)
#endif
#ifndef LAB02_LARGE_WORK
#define LAB02_LARGE_WORK UINT32_C(120000)
#endif

#define LAB02_N 3U
#define LAB02_NONE UINT8_MAX
#define LAB02_HALF_RANGE UINT32_C(0x80000000)

typedef enum {
    LAB02_SHORT = 0,
    LAB02_LONG_B,
    LAB02_VARIABLE_B,
    LAB02_SLEEP_A
} Lab02_Mode;

typedef enum { ST_READY, ST_BLOCKED } TaskState;

typedef struct {
    void (*step)(void);
    TaskState state;
    uint32_t wake_at;
} Lab02_TCB;

typedef struct {
    uint32_t runs[LAB02_N];
    uint32_t result[LAB02_N];
    uint32_t idle_entries;
    uint32_t failures;
} Lab02_Diagnostics;

/* Normal runs/result/idle counters are written by main and its steps.
   failures may also be written by a rejected handler-context call. Read a
   coherent snapshot while CM4 is halted. Counters wrap modulo 2^32 and are
   diagnostics, not timing measurements or a guaranteed count of all faults. */
extern volatile Lab02_Diagnostics lab02_diag;

/* Initialize once after dual-core boot, HAL and GPIO setup. Returns 1 on
   success, 0 for repeated/invalid setup or incompatible HAL tick frequency.
   The marker and application must have exactly one owner, CM4. */
int Lab02_Init(Lab02_Mode selected);
/* One selection and at most one step; never call from an ISR or another step. */
void Lab02_DispatchOnce(void);
/* Valid only inside a step, for 0 < ms < 2^31. After success, the caller must
   return itself. This sets readiness metadata; it does not suspend a stack. */
int Lab02_SleepAfterStep(uint32_t ms);
/* Modulo comparison: all relevant intervals/overdue ages must be < 2^31 ms. */
int Lab02_TimeReached(uint32_t now, uint32_t target);

#endif
