#include "main.h"
#include "lab08_reset.h"
volatile uint32_t lab08_reset_flags;
static int captured;
void Lab08_CM7_CaptureReset(void)
{
    if (!captured) {
        lab08_reset_flags = RCC->RSR;
        __HAL_RCC_CLEAR_RESET_FLAGS();
        captured = 1;
    }
}
