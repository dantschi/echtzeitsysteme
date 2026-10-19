/* Student starting point: replace both bodies. The initial worker only waits. */
#include "lab05_button.h"
void Lab05_QueueButton(void) {}
void Lab05_PulseWorker(void *argument)
{
    (void)argument;
    for (;;) { Lab05_ButtonCheck(osDelay(1000U)); }
}
