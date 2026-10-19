/* One CM4 CMSIS-RTOS2 worker, same physical ipc as CM7.
 * Requires non-cacheable shared memory for the baseline and a working
 * generated boot/kernel. Do not initialize ipc here or in CM4 startup. */
#include "main.h"
#include "cmsis_os2.h"
#include "lab06_ipc.h"
#ifdef LAB06_STUDENT_EXERCISE
#include "lab06_student.h"
#endif

#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "lab06_cm4.c is only for the CM4 target"
#endif

#define WAIT_TIMEOUT_TICKS UINT32_C(2000)

volatile Lab06_Error lab06_error;
volatile uint32_t lab06_cm4_responses;
volatile uint32_t lab06_cm4_done;
static int run_started;

static void Fail(Lab06_Error error)
{
    lab06_error = error;
    Error_Handler();
    for (;;) {}
}

void Lab06_CM4_Run(void *argument)
{
    (void)argument;
    if (run_started || osKernelGetState() != osKernelRunning) {
        Fail(LAB06_ERROR_SETUP);
    }
    run_started = 1;
    for (uint32_t round = 0u; round < LAB06_ROUNDS; ++round) {
        uint32_t wait_begin = osKernelGetTickCount();
        for (;;) {
            if (ipc.magic != IPC_MAGIC) {
                Fail(LAB06_ERROR_MAGIC);
            }
            uint32_t state = ipc.state;
            if (state == REQUEST_M4) {
                break;
            }
            if (state != RESPONSE_M7) {
                Fail(LAB06_ERROR_STATE);
            }
            if ((uint32_t)(osKernelGetTickCount() - wait_begin)
                >= WAIT_TIMEOUT_TICKS) {
                Fail(LAB06_ERROR_TIMEOUT);
            }
            if (osDelay(1u) != osOK) {
                Fail(LAB06_ERROR_RTOS);
            }
        }
        __DMB();
        uint32_t request = ipc.payload;
        #ifdef LAB06_STUDENT_EXERCISE
        if (request != Lab06_ExerciseRequest(round)) {
#else
        if (request != 2u * round) {
#endif
            Fail(LAB06_ERROR_PAYLOAD);
        }
#ifdef LAB06_STUDENT_EXERCISE
        Lab06_Respond();
        if (ipc.state != RESPONSE_M7) { Fail(LAB06_ERROR_STATE); }
#else
        ipc.payload = request + 1u;
        __DMB();
        ipc.state = RESPONSE_M7;
        __DMB();
#endif
        lab06_cm4_responses = round + 1u;
    }
    lab06_cm4_done = 1u;
    osThreadExit();
    Fail(LAB06_ERROR_RTOS); /* Guard if an incompatible adapter returns. */
}
