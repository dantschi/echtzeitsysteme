/* Student starting point for lab 3, task 6.
   Replace both entry-function bodies with your own application logic.
   Until then, the initial project starts both threads but performs no
   sampling or signalling. The temporary bodies block instead of spinning. */
#include "lab03_tank.h"

static _Noreturn void WaitForImplementation(void)
{
    for (;;) {
        if (osDelay(1000U) != osOK) {
            Lab03_TankFail();
        }
    }
}

void Lab03_TankSampler(void *argument)
{
    (void)argument;
    WaitForImplementation();
}

void Lab03_TankAlarm(void *argument)
{
    (void)argument;
    WaitForImplementation();
}
