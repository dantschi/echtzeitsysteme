/* Student starting point for lab 3, task 6.
   Replace both entry-function bodies with your own application logic.
   Until then the threads only block: no computation or LED action.
   Remove WaitForImplementation after implementing both functions. */
#include "lab03_rtos.h"

static _Noreturn void WaitForImplementation(void)
{
    for (;;) {
        if (osDelay(1000U) != osOK) {
            Lab03_Fail();
        }
    }
}

void Lab03_Heartbeat(void *argument)
{
    (void)argument;
    WaitForImplementation();
}

void Lab03_HeavyMath(void *argument)
{
    (void)argument;
    WaitForImplementation();
}
