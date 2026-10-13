/* CM7 application, NUCLEO-H755ZI-Q; a working CubeMX boot is required.
 * Shared SRAM4 is reserved exclusively for IPC in both images.
 * Baseline: MPU Normal/non-cacheable, TEX=1 C=0 B=0 S=1, XN.
 * Fault build only: TEX=1 C=1 B=1 S=0 with D-cache enabled, no maintenance.
 * Change attributes ONLY at joint restart; never during communication. */
#include "main.h"
#include "lab06_ipc.h"

#if !defined(CORE_CM7) || defined(CORE_CM4)
#error "lab06_cm7.c is only for the CM7 target"
#endif

#define WAIT_TIMEOUT_MS UINT32_C(2000)

volatile Lab06_Error lab06_error;
volatile uint32_t lab06_cm7_reply[LAB06_ROUNDS];
volatile uint32_t lab06_cm7_done;
static int memory_configured;
static int shared_initialized;
static int run_started;

static void Fail(Lab06_Error error)
{
    lab06_error = error;
    Error_Handler();
    for (;;) {}
}

void Lab06_CM7_ConfigureMemory(uint32_t region_number, Lab06_MemoryMode mode)
{
    uint32_t region_count = (MPU->TYPE & MPU_TYPE_DREGION_Msk)
                            >> MPU_TYPE_DREGION_Pos;
    if (memory_configured || shared_initialized || region_number >= region_count ||
        region_number > 15u || (SCB->CCR & SCB_CCR_DC_Msk) != 0u ||
        (mode != LAB06_NONCACHED && mode != LAB06_CACHED_FAULT)) {
        Fail(LAB06_ERROR_SETUP);
    }
    MPU_Region_InitTypeDef region = {0};
    region.Enable = MPU_REGION_ENABLE;
    region.Number = (uint8_t)region_number;
    region.BaseAddress = LAB06_SHARED_ADDRESS;
    region.Size = MPU_REGION_SIZE_64KB;
    region.SubRegionDisable = 0u;
    region.TypeExtField = MPU_TEX_LEVEL1;
    region.AccessPermission = MPU_REGION_FULL_ACCESS;
    region.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    region.IsShareable = mode == LAB06_NONCACHED ? MPU_ACCESS_SHAREABLE
                                               : MPU_ACCESS_NOT_SHAREABLE;
    region.IsCacheable = mode == LAB06_NONCACHED ? MPU_ACCESS_NOT_CACHEABLE
                                               : MPU_ACCESS_CACHEABLE;
    region.IsBufferable = mode == LAB06_NONCACHED ? MPU_ACCESS_NOT_BUFFERABLE
                                                : MPU_ACCESS_BUFFERABLE;
    /* Existing regions remain. Caller must reserve this free region number
     * and verify effective attributes, including all overlapping regions. */
    HAL_MPU_Disable();
    HAL_MPU_ConfigRegion(&region);
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
    __DSB();
    __ISB();
    memory_configured = 1;
}

void Lab06_CM7_InitShared(void)
{
    if (!memory_configured || shared_initialized ||
        (SCB->CCR & SCB_CCR_DC_Msk) != 0u) {
        Fail(LAB06_ERROR_SETUP);
    }
    /* CM4 is still stopped in the agreed generated boot handshake.
     * D-cache OFF: even the fault build has initial bytes in SRAM. */
    ipc.magic = 0u;
    ipc.state = TURN_M7;
    ipc.payload = 0u;
    __DMB();
    ipc.magic = IPC_MAGIC;
    __DMB();
    ipc.state = REQUEST_M4;
    __DMB();
    shared_initialized = 1;
}

void Lab06_CM7_Run(void)
{
    if (!shared_initialized || run_started) {
        Fail(LAB06_ERROR_SETUP);
    }
    run_started = 1;
    for (uint32_t round = 0u; round < LAB06_ROUNDS; ++round) {
        uint32_t wait_begin = HAL_GetTick();
        for (;;) {
            if (ipc.magic != IPC_MAGIC) {
                Fail(LAB06_ERROR_MAGIC);
            }
            uint32_t state = ipc.state;
            if (state == RESPONSE_M7) {
                break;
            }
            if (state != REQUEST_M4) {
                Fail(LAB06_ERROR_STATE);
            }
            if ((uint32_t)(HAL_GetTick() - wait_begin) >= WAIT_TIMEOUT_MS) {
                Fail(LAB06_ERROR_TIMEOUT);
            }
            /* Throttle polling; active HAL wait, not an IPC latency measure. */
            HAL_Delay(1u);
        }
        __DMB(); /* State observed before reading the associated payload. */
        uint32_t reply = ipc.payload;
        if (reply != 2u * round + 1u) {
            Fail(LAB06_ERROR_PAYLOAD);
        }
        lab06_cm7_reply[round] = reply;
        if (round + 1u < LAB06_ROUNDS) {
            ipc.payload = reply + 1u;
            __DMB();
            ipc.state = REQUEST_M4;
            __DMB();
        } else {
            /* No fourth request. CM4 has finished its third response. */
            __DMB();
            ipc.state = TURN_M7;
            __DMB();
        }
    }
    lab06_cm7_done = 1u;
}
