#ifndef LAB04_RESOURCES_H
#define LAB04_RESOURCES_H

#include <stdint.h>
#include "cmsis_os2.h"

typedef enum {
    LAB04_SEMAPHORE = 0,
    LAB04_MUTEX = 1
} Lab04_Mode;

enum {
    LAB04_LOW = 0,
    LAB04_MEDIUM = 1,
    LAB04_HIGH = 2
};

typedef enum {
    LAB04_NOT_STARTED = 0,
    LAB04_GATE_WAIT = 1,
    LAB04_LOCK_REQUEST = 2,
    LAB04_LOCK_ACQUIRED = 3,
    LAB04_WORK = 4,
    LAB04_FINISHED = 5
} Lab04_Stage;

typedef struct {
    uint32_t started_tick;
    uint32_t gate_release_request_tick;
    uint32_t request_tick;
    uint32_t acquired_tick;
    uint32_t release_request_tick;
    uint32_t done_tick;
    Lab04_Stage stage;
} Lab04_Log;

/* Call once after osKernelInitialize(), before creating the L/M/H threads. */
void Lab04_Init(Lab04_Mode mode);

/* Call from the generated CM4 entry functions. These do not return. */
void Lab04_Low(void *argument);
void Lab04_Medium(void *argument);
void Lab04_High(void *argument);

/* Each slot is written by its own thread only. Inspect after the run. */
extern volatile Lab04_Log lab04_log[3];
extern volatile uint32_t lab04_result[3];
extern volatile osPriority_t lab04_low_effective_priority;

#endif
