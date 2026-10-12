#ifndef LAB05_MESSAGES_H
#define LAB05_MESSAGES_H

#include <stdint.h>
#include "cmsis_os2.h"

#define LAB05_ATTEMPTS 20u
#define LAB05_QUEUE_CAPACITY 4u

typedef enum {
    LAB05_DATA = 0,
    LAB05_STOP = 1
} Lab05_Kind;

typedef struct {
    uint32_t sample;
    uint32_t seq;
    uint32_t tick;
    Lab05_Kind kind;
} Lab05_Message;

typedef struct {
    uint32_t tick;
    osStatus_t status;
} Lab05_Attempt;

typedef struct {
    Lab05_Message message;
    uint32_t received_tick;
} Lab05_Received;

typedef struct {
    uint32_t seq;
    uint32_t tick;
} Lab05_ButtonEvent;

/* Call once after the existing osKernelInitialize(), before osKernelStart(). */
void Lab05_Init(void);

/* Call from the corresponding generated CM4 thread entry functions. */
void Lab05_Producer(void *argument);
void Lab05_Consumer(void *argument);
void Lab05_ButtonWorker(void *argument);

/* Only the one validated CM4 button IRQ may call this after kernel start.
 * The function cannot check NVIC priority: configure it before enabling IRQ.
 * Do not also call it from a thread, another IRQ or the CM7 callback. */
void Lab05_ButtonIRQ(void);

/* Debugger diagnostics, not a concurrent reporting API.
 * Producer and Consumer each write only their own fields/arrays.
 * Read consistent Part A values after both done flags and halted execution.
 * For Part B halt all cores or stop IRQ and drain before interpreting totals.
 * volatile does not provide synchronization for other application readers. */
extern volatile Lab05_Attempt lab05_attempt[LAB05_ATTEMPTS];
extern volatile Lab05_Received lab05_received[LAB05_ATTEMPTS];
extern volatile uint32_t lab05_accepted;
extern volatile uint32_t lab05_dropped;
extern volatile uint32_t lab05_received_count;
extern volatile uint32_t lab05_producer_done;
extern volatile uint32_t lab05_consumer_done;
extern volatile uint32_t lab05_button_offered;
extern volatile uint32_t lab05_button_accepted;
extern volatile uint32_t lab05_button_dropped;
extern volatile uint32_t lab05_button_errors;
extern volatile uint32_t lab05_button_processed;
extern volatile Lab05_ButtonEvent lab05_button_last;
extern volatile uint32_t lab05_button_last_received_tick;

#endif
