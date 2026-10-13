#ifndef LAB08_RESET_H
#define LAB08_RESET_H
#include <stdint.h>
#if !defined(CORE_CM7) || defined(CORE_CM4)
#error "CM7 is the sole reset-flag capture/clear owner in this experiment"
#endif
extern volatile uint32_t lab08_reset_flags;
/* Once per boot, before HAL/project code clears reset flags and before
 * D-cache enable. CM4 must not clear the shared RCC reset history. */
void Lab08_CM7_CaptureReset(void);
#endif
