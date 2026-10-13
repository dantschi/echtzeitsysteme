#ifndef LAB06_IPC_H
#define LAB06_IPC_H

#include <stddef.h>
#include <stdint.h>

#define IPC_MAGIC UINT32_C(0x49504336)
#define TURN_M7 UINT32_C(1)
#define REQUEST_M4 UINT32_C(2)
#define RESPONSE_M7 UINT32_C(3)
#define LAB06_ROUNDS 3u
#define LAB06_SHARED_ADDRESS UINT32_C(0x38000000)
#define LAB06_CACHE_LINE 32u

typedef struct {
    uint32_t magic;
    uint32_t state;
    uint32_t payload;
} ipc_block_t;

_Static_assert(sizeof(ipc_block_t) == 12u, "IPC type size");
_Static_assert(offsetof(ipc_block_t, magic) == 0u, "magic offset");
_Static_assert(offsetof(ipc_block_t, state) == 4u, "state offset");
_Static_assert(offsetof(ipc_block_t, payload) == 8u, "payload offset");
_Static_assert(_Alignof(ipc_block_t) >= 4u, "IPC word alignment");

/* Same physical address in both images; one definition per image.
 * This is a platform protocol for aligned Armv7-M accesses, not a portable
 * ISO C concurrency primitive. DMB, MPU and ownership are all required. */
extern volatile ipc_block_t ipc;

typedef enum {
    LAB06_NONCACHED = 0,
    LAB06_CACHED_FAULT = 1
} Lab06_MemoryMode;

typedef enum {
    LAB06_ERROR_NONE = 0,
    LAB06_ERROR_SETUP = 1,
    LAB06_ERROR_MAGIC = 2,
    LAB06_ERROR_STATE = 3,
    LAB06_ERROR_PAYLOAD = 4,
    LAB06_ERROR_TIMEOUT = 5,
    LAB06_ERROR_RTOS = 6
} Lab06_Error;

/* Diagnostics are private globals in each image, outside .shared.
 * Observe with all cores halted; no concurrent reporting thread/ISR. */
extern volatile Lab06_Error lab06_error;

#if defined(CORE_CM7) && !defined(CORE_CM4)
/* Use a free region with priority above overlapping lower-numbered regions.
 * Call after the project's MPU setup, before D-cache is enabled, at boot.
 * The helper does not detect overlap or enforce policy for the other core. */
void Lab06_CM7_ConfigureMemory(uint32_t region_number, Lab06_MemoryMode mode);
/* Exactly once, after clock setup and before D-cache enable / boot release. */
void Lab06_CM7_InitShared(void);
/* Exactly once, after CM4 release; HAL's ms time base must run. Returns. */
void Lab06_CM7_Run(void);
extern volatile uint32_t lab06_cm7_reply[LAB06_ROUNDS];
extern volatile uint32_t lab06_cm7_done;
#elif defined(CORE_CM4) && !defined(CORE_CM7)
/* Call from the one generated CMSIS-RTOS2 worker; terminates via osThreadExit.
 * Kernel, heap, stack and one-tick wait support must be configured. */
void Lab06_CM4_Run(void *argument);
extern volatile uint32_t lab06_cm4_responses;
extern volatile uint32_t lab06_cm4_done;
#else
#error "Build each image with exactly one of CORE_CM7 and CORE_CM4"
#endif

#endif
