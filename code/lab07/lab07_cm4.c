/* One CM4 CMSIS-RTOS2 worker, same physical ipc as CM7.
 * Requires non-cacheable shared memory for the baseline and a working
 * generated boot/kernel. Do not initialize ipc here or in CM4 startup. */
#include "main.h"
#include "cmsis_os2.h"
#include "lab07_ipc.h"
#ifdef LAB07_STUDENT_EXERCISE
#include "lab07_student.h"
#endif

#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "lab07_cm4.c is only for the CM4 target"
#endif

#define WAIT_TIMEOUT_TICKS UINT32_C(2000)

volatile Lab07_Error lab07_error;
volatile uint32_t lab07_cm4_responses;
volatile uint32_t lab07_cm4_done;
static int run_started;

static void Fail(Lab07_Error error)
{
    lab07_error = error;
    Error_Handler();
    for (;;) {}
}

void Lab07_CM4_Run(void *argument)
{
    (void)argument;
    if (run_started || osKernelGetState() != osKernelRunning) {
        Fail(LAB07_ERROR_SETUP);
    }
    run_started = 1;
    for (uint32_t round = 0u; round < LAB07_ROUNDS; ++round) {
        uint32_t wait_begin = osKernelGetTickCount();
        for (;;) {
            if (ipc.magic != IPC_MAGIC) {
                Fail(LAB07_ERROR_MAGIC);
            }
            uint32_t state = ipc.state;
            if (state == REQUEST_M4) {
                break;
            }
            if (state != RESPONSE_M7) {
                Fail(LAB07_ERROR_STATE);
            }
            if ((uint32_t)(osKernelGetTickCount() - wait_begin)
                >= WAIT_TIMEOUT_TICKS) {
                Fail(LAB07_ERROR_TIMEOUT);
            }
            if (osDelay(1u) != osOK) {
                Fail(LAB07_ERROR_RTOS);
            }
        }
        __DMB();
        uint32_t request = ipc.payload;
        #ifdef LAB07_STUDENT_EXERCISE
        if (request != Lab07_ExerciseRequest(round)) {
#else
        if (request != 2u * round) {
#endif
            Fail(LAB07_ERROR_PAYLOAD);
        }
#ifdef LAB07_STUDENT_EXERCISE
        Lab07_Respond();
        if (ipc.state != RESPONSE_M7) { Fail(LAB07_ERROR_STATE); }
#else
        ipc.payload = request + 1u;
        __DMB();
        ipc.state = RESPONSE_M7;
        __DMB();
#endif
        lab07_cm4_responses = round + 1u;
    }
    lab07_cm4_done = 1u;
    osThreadExit();
    Fail(LAB07_ERROR_RTOS); /* Guard if an incompatible adapter returns. */
}
