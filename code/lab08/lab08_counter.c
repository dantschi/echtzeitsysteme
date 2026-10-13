#include "main.h"
#include "lab08_counter.h"
#if defined(CORE_CM4)
#include "cmsis_os2.h"
#endif

volatile Lab08_Error lab08_error;
volatile uint32_t lab08_hits, lab08_misses;
volatile uint32_t lab08_final, lab08_expected, lab08_complete;
static int run_started;
#if defined(CORE_CM7)
static int memory_configured, shared_initialized;
#endif
static void Fail(Lab08_Error error)
{
    lab08_error = error;
    Error_Handler();
    for (;;) {}
}
static void CheckProtocol(void)
{
    if (lab08_shared.magic != LAB08_MAGIC ||
        lab08_shared.mode > (uint32_t)LAB08_UNPROTECTED) {
        Fail(LAB08_PROTOCOL);
    }
}
static void Worker(uint32_t index, uint32_t process_id)
{
    if (run_started) { Fail(LAB08_SETUP); }
    run_started = 1;
    CheckProtocol();
    __DMB();
    const uint32_t mode = lab08_shared.mode;
    uint32_t hits = 0u, misses = 0u;
    for (uint32_t attempt = 0u; attempt < LAB08_ATTEMPTS; ++attempt) {
        if (mode == (uint32_t)LAB08_UNPROTECTED) {
            /* Intentional fault build: read/modify/write is NOT atomic. */
            lab08_shared.count += 1u;
            ++hits;
        } else if (HAL_HSEM_Take(LAB08_HSEM_ID, process_id) == HAL_OK) {
            __DMB();
            lab08_shared.count += 1u;
            __DMB();
            HAL_HSEM_Release(LAB08_HSEM_ID, process_id);
            ++hits;
        } else {
            ++misses;
        }
        /* Never delay with the lock held. No ISR/task shares this owner. */
#if defined(CORE_CM7)
        HAL_Delay(1u);
#else
        if (osDelay(1u) != osOK) { Fail(LAB08_SETUP); }
#endif
    }
    lab08_hits = hits;
    lab08_misses = misses;
    lab08_shared.hits[index] = hits;
    __DMB();
    lab08_shared.done[index] = 1u;
    __DMB();
}
#if defined(CORE_CM7)
void Lab08_CM7_ConfigureMemory(uint32_t free_region)
{
    uint32_t count = (MPU->TYPE & MPU_TYPE_DREGION_Msk) >> MPU_TYPE_DREGION_Pos;
    if (memory_configured || shared_initialized || free_region >= count ||
        free_region > 15u || (SCB->CCR & SCB_CCR_DC_Msk) != 0u) {
        Fail(LAB08_SETUP);
    }
    MPU_Region_InitTypeDef region = {0};
    region.Enable = MPU_REGION_ENABLE;
    region.Number = (uint8_t)free_region;
    region.BaseAddress = LAB08_SHARED_ADDRESS;
    region.Size = MPU_REGION_SIZE_64KB;
    region.SubRegionDisable = 0u;
    region.TypeExtField = MPU_TEX_LEVEL1;
    region.AccessPermission = MPU_REGION_FULL_ACCESS;
    region.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    region.IsShareable = MPU_ACCESS_SHAREABLE;
    region.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    region.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    HAL_MPU_Disable();
    HAL_MPU_ConfigRegion(&region);
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
    __DSB();
    __ISB();
    memory_configured = 1;
}
void Lab08_CM7_InitShared(Lab08_Mode mode)
{
    if (!memory_configured || shared_initialized ||
        (SCB->CCR & SCB_CCR_DC_Msk) != 0u ||
        (mode != LAB08_PROTECTED && mode != LAB08_UNPROTECTED)) {
        Fail(LAB08_SETUP);
    }
    __HAL_RCC_HSEM_CLK_ENABLE();
    lab08_shared.magic = 0u;
    lab08_shared.mode = (uint32_t)mode;
    lab08_shared.count = 0u;
    for (uint32_t index = 0u; index < 2u; ++index) {
        lab08_shared.hits[index] = 0u;
        lab08_shared.done[index] = 0u;
    }
    __DMB();
    lab08_shared.magic = LAB08_MAGIC;
    __DMB();
    shared_initialized = 1;
}
void Lab08_CM7_Run(void)
{
    if (!shared_initialized) { Fail(LAB08_SETUP); }
    Worker(0u, 0x37u);
    const uint32_t begin = HAL_GetTick();
    for (;;) {
        CheckProtocol();
        const uint32_t done = lab08_shared.done[1];
        if (done == 1u) { break; }
        if (done != 0u) { Fail(LAB08_PROTOCOL); }
        if ((uint32_t)(HAL_GetTick() - begin) >= 2000u) {
            Fail(LAB08_TIMEOUT);
        }
        HAL_Delay(1u);
    }
    __DMB();
    CheckProtocol();
    if (lab08_shared.hits[0] > LAB08_ATTEMPTS ||
        lab08_shared.hits[1] > LAB08_ATTEMPTS) { Fail(LAB08_PROTOCOL); }
    lab08_expected = lab08_shared.hits[0] + lab08_shared.hits[1];
    lab08_final = lab08_shared.count;
    if (lab08_shared.mode == (uint32_t)LAB08_PROTECTED &&
        lab08_final != lab08_expected) { Fail(LAB08_SUM); }
    lab08_complete = 1u;
}
#else
void Lab08_CM4_Run(void *argument)
{
    (void)argument;
    if (osKernelGetState() != osKernelRunning) { Fail(LAB08_SETUP); }
    Worker(1u, 0x34u);
    lab08_complete = 1u;
    osThreadExit();
    Fail(LAB08_SETUP); /* osThreadExit must not return. */
}
#endif
