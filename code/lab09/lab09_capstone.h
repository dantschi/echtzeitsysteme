#ifndef LAB09_CAPSTONE_H
#define LAB09_CAPSTONE_H
#include <stddef.h>
#include <stdint.h>
#if (defined(CORE_CM7) && defined(CORE_CM4)) || \
    (!defined(CORE_CM7) && !defined(CORE_CM4))
#error "Build each image with exactly one core flag"
#endif
#define LAB09_MAGIC UINT32_C(0x49504339)
#define LAB09_HSEM_ID 1u
#define LAB09_MAX_EVENTS 20u
#define LAB09_SHARED_ADDRESS UINT32_C(0x38000000)
#define LAB09_MAX_SAMPLE 1023u

typedef enum { LAB09_NORMAL = 0, LAB09_SATURATION = 1 } Lab09_Mode;
typedef enum {
    LAB09_OK = 0, LAB09_SETUP, LAB09_PROTOCOL, LAB09_LOCK_TIMEOUT,
    LAB09_KERNEL, LAB09_SOURCE, LAB09_OUTPUT, LAB09_LOG_FULL
} Lab09_Error;
typedef struct {
    uint32_t magic, mode, valid;
    uint32_t event_seq, pub_seq, event_tick, payload;
    uint32_t ack_pub, overwritten, snapshots, producer_done;
} Lab09_Box;
_Static_assert(sizeof(Lab09_Box) == 44u, "Mailbox type size");
_Static_assert(offsetof(Lab09_Box, event_seq) == 12u, "Event offset");
_Static_assert(offsetof(Lab09_Box, ack_pub) == 28u, "Ack offset");
_Static_assert(offsetof(Lab09_Box, producer_done) == 40u, "Done offset");
extern volatile Lab09_Box lab09_box;
/* Call with HSEM held, after acquire DMB, or after all writers have stopped.
 * Bounded, no sequence wrap.
 * Counter relations are checked at transaction boundaries, not mid-update. */
static inline int Lab09_BoxValid(void)
{
    const uint32_t pub = lab09_box.pub_seq;
    if (lab09_box.magic != LAB09_MAGIC || lab09_box.mode > LAB09_SATURATION ||
        pub > LAB09_MAX_EVENTS || lab09_box.valid != (uint32_t)(pub != 0u) ||
        lab09_box.ack_pub > pub || lab09_box.producer_done > 1u ||
        lab09_box.overwritten > pub || lab09_box.snapshots > pub) { return 0; }
    const uint32_t unread = pub != 0u && lab09_box.ack_pub != pub ? 1u : 0u;
    return pub == lab09_box.overwritten + lab09_box.snapshots + unread;
}

/* Private per-image diagnostics; observe only at a controlled halt. */
extern volatile Lab09_Error lab09_error;
extern volatile uint32_t lab09_complete;
typedef struct { uint32_t event_seq, pub_seq, event_tick, sample; } Lab09_Record;

#if defined(CORE_CM7)
typedef struct { Lab09_Record record; uint32_t filtered; } Lab09_Result;
extern volatile Lab09_Result lab09_results[LAB09_MAX_EVENTS];
extern volatile uint32_t lab09_result_count;
void Lab09_CM7_ConfigureMemory(uint32_t free_region);
/* CM4 still stopped by the existing boot. D-cache off. One call per boot. */
void Lab09_CM7_InitShared(Lab09_Mode mode);
/* 1 ms baseline; 25 ms intentionally slower comparison. One call per boot.
 * Deadline timer requires running HAL millisecond tick. No RTOS on CM7. */
void Lab09_CM7_Run(uint32_t pause_ms);
/* Supplied output adapter: RAM-only default, optional bounded UART. */
int Lab09_CM7_Output(const Lab09_Result *result);
#else
typedef struct {
    uint32_t inputs, accepted, queue_drops, put_errors;
    uint32_t queued, in_flight, published, task_drops;
} Lab09_Stats;
extern volatile Lab09_Stats lab09_stats;
extern volatile Lab09_Record lab09_publications[LAB09_MAX_EVENTS];
extern volatile uint32_t lab09_publication_count;
/* Sole CMSIS-RTOS2 worker. Creates queue, enables source after queue exists,
 * consumes finite input, disables source, deletes queue and exits thread. */
void Lab09_CM4_Run(void *argument);
/* Called once per generated timer event (or controlled saturation call).
 * ISR use requires allowed NVIC priority. Timeout zero, never HSEM in ISR. */
void Lab09_CM4_OnEvent(uint32_t sample);
/* Provided in lab09_timer.c, only used by NORMAL mode. */
int Lab09_CM4_StartEvents(void);
int Lab09_CM4_StopEvents(void);
#endif
#endif
