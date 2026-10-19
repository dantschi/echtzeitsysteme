/* Student starting point. Use INSTEAD OF lab09_output.c in CM7.
   Implement hysteresis and publication-gap LEDs; no UART or external pins. */
#include "main.h"
#include "lab09_capstone.h"
#if !defined(CORE_CM7) || defined(CORE_CM4)
#error "LED adapter requires CM7"
#endif
int Lab09_CM7_Output(const Lab09_Result *result)
{
    (void)result;
    return 1; /* Prepared data path runs; display is not implemented yet. */
}
