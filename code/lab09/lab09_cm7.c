#include "main.h"
#include "lab09_capstone.h"
#if !defined(CORE_CM7) || defined(CORE_CM4)
#error "CM7 source only"
#endif
volatile Lab09_Error lab09_error;
volatile uint32_t lab09_complete;
volatile Lab09_Result lab09_results[LAB09_MAX_EVENTS];
volatile uint32_t lab09_result_count;
static int memory_configured, shared_initialized, started;
static void Fail(Lab09_Error error)
{
    lab09_error = error;
    Error_Handler();
    for (;;) {}
}
void Lab09_CM7_ConfigureMemory(uint32_t free_region)
{
    uint32_t count = (MPU->TYPE & MPU_TYPE_DREGION_Msk) >> MPU_TYPE_DREGION_Pos;
    if (memory_configured || shared_initialized || free_region >= count ||
        free_region > 15u || (SCB->CCR & SCB_CCR_DC_Msk) != 0u) {
        Fail(LAB09_SETUP);
    }
    MPU_Region_InitTypeDef region = {0};
    region.Enable = MPU_REGION_ENABLE;
    region.Number = (uint8_t)free_region;
    region.BaseAddress = LAB09_SHARED_ADDRESS;
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
void Lab09_CM7_InitShared(Lab09_Mode mode)
{
    if (!memory_configured || shared_initialized ||
        (SCB->CCR & SCB_CCR_DC_Msk) != 0u ||
        (mode != LAB09_NORMAL && mode != LAB09_SATURATION)) { Fail(LAB09_SETUP); }
    __HAL_RCC_HSEM_CLK_ENABLE();
    lab09_box.magic = 0u;
    lab09_box.mode = (uint32_t)mode;
    lab09_box.valid = 0u;
    lab09_box.event_seq = 0u;
    lab09_box.pub_seq = 0u;
    lab09_box.event_tick = 0u;
    lab09_box.payload = 0u;
    lab09_box.ack_pub = 0u;
    lab09_box.overwritten = 0u;
    lab09_box.snapshots = 0u;
    lab09_box.producer_done = 0u;
    __DMB();
    lab09_box.magic = LAB09_MAGIC;
    __DMB();
    shared_initialized = 1;
}
void Lab09_CM7_Run(uint32_t pause_ms)
{
    if (!shared_initialized || started || pause_ms < 1u || pause_ms > 100u) {
        Fail(LAB09_SETUP);
    }
    started = 1;
    uint32_t ring[4] = {0u}, sum = 0u, used = 0u, position = 0u, last = 0u;
    const uint32_t begin = HAL_GetTick();
    for (;;) {
        int fresh = 0, done = 0;
        Lab09_Result result = {0};
        if (HAL_HSEM_Take(LAB09_HSEM_ID, 0x37u) == HAL_OK) {
            __DMB();
            if (!Lab09_BoxValid() || lab09_box.pub_seq < last ||
                lab09_box.ack_pub != last) {
                __DMB(); HAL_HSEM_Release(LAB09_HSEM_ID, 0x37u);
                Fail(LAB09_PROTOCOL);
            }
            done = lab09_box.producer_done == 1u;
            if (lab09_box.valid != 0u && lab09_box.pub_seq != last) {
                result.record.event_seq = lab09_box.event_seq;
                result.record.pub_seq = lab09_box.pub_seq;
                result.record.event_tick = lab09_box.event_tick;
                result.record.sample = lab09_box.payload;
                last = result.record.pub_seq;
                lab09_box.ack_pub = last;
                ++lab09_box.snapshots;
                fresh = 1;
            }
            __DMB();
            HAL_HSEM_Release(LAB09_HSEM_ID, 0x37u);
        }
        if (fresh) {
            if (result.record.sample > LAB09_MAX_SAMPLE) { Fail(LAB09_PROTOCOL); }
            if (lab09_result_count >= LAB09_MAX_EVENTS) { Fail(LAB09_LOG_FULL); }
            sum -= ring[position];
            ring[position] = result.record.sample;
            sum += ring[position];
            position = (position + 1u) % 4u;
            if (used < 4u) { ++used; }
            result.filtered = sum / used; /* Unsigned integer floor, no overflow. */
            uint32_t slot = lab09_result_count;
            lab09_results[slot] = result;
            ++lab09_result_count;
            if (!Lab09_CM7_Output(&result)) { Fail(LAB09_OUTPUT); }
        }
        /* Check the original total deadline before accepting completion.
         * Includes lock failures, filtering and output; no late success. */
        if ((uint32_t)(HAL_GetTick() - begin) >= 3000u) { Fail(LAB09_LOCK_TIMEOUT); }
        if (done) { lab09_complete = 1u; return; }
        HAL_Delay(pause_ms);
    }
}
