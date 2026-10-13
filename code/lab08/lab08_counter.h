#ifndef LAB08_COUNTER_H
#define LAB08_COUNTER_H
#include <stddef.h>
#include <stdint.h>

#if (defined(CORE_CM7) && defined(CORE_CM4)) || \
    (!defined(CORE_CM7) && !defined(CORE_CM4))
#error "Select exactly one of CORE_CM7 and CORE_CM4 for each image"
#endif
#define LAB08_HSEM_ID 1u
#define LAB08_ATTEMPTS 100u
#define LAB08_MAGIC UINT32_C(0x49504338)
#define LAB08_SHARED_ADDRESS UINT32_C(0x38000000)

typedef enum { LAB08_PROTECTED = 0, LAB08_UNPROTECTED = 1 } Lab08_Mode;
typedef enum {
    LAB08_OK = 0, LAB08_SETUP, LAB08_PROTOCOL, LAB08_TIMEOUT, LAB08_SUM
} Lab08_Error;
typedef struct {
    uint32_t magic;
    uint32_t mode;
    uint32_t count;
    uint32_t hits[2];
    uint32_t done[2];
} Lab08_Shared;
_Static_assert(sizeof(Lab08_Shared) == 28u, "Shared type size");
_Static_assert(offsetof(Lab08_Shared, count) == 8u, "Counter offset");
_Static_assert(offsetof(Lab08_Shared, hits) == 12u, "Hits offset");
_Static_assert(offsetof(Lab08_Shared, done) == 20u, "Done offset");
extern volatile Lab08_Shared lab08_shared;
/* Private per-image diagnostics: inspect with BOTH cores halted. */
extern volatile Lab08_Error lab08_error;
extern volatile uint32_t lab08_hits, lab08_misses;
extern volatile uint32_t lab08_final, lab08_expected, lab08_complete;

#if defined(CORE_CM7)
/* Exactly once at boot, D-cache OFF. Reserve an actually free region with
 * priority above conflicting overlaps. Other MPU regions remain in place. */
void Lab08_CM7_ConfigureMemory(uint32_t free_region);
/* Exactly once, CM4 still stopped by the EXISTING generated boot handshake.
 * Replace the lab06 shared object; do not link both examples together. */
void Lab08_CM7_InitShared(Lab08_Mode mode);
/* After existing CM4 boot release; running HAL millisecond timebase. */
void Lab08_CM7_Run(void);
#else
/* The sole generated CMSIS-RTOS2 worker. Terminates through osThreadExit. */
void Lab08_CM4_Run(void *argument);
#endif
#endif
