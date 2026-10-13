#ifndef LAB08_WWDG_H
#define LAB08_WWDG_H
#include <stdint.h>
#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "WWDG2 example is only for CORE_CM4"
#endif

typedef enum {
    LAB08_WWDG_EARLY = 0, LAB08_WWDG_VALID = 1, LAB08_WWDG_LATE = 2
} Lab08_WWDG_Mode;
typedef struct {
    Lab08_WWDG_Mode mode;
    uint32_t irq_priority; /* Project-selected unshifted NVIC priority. */
} Lab08_WWDG_Config;
extern volatile uint32_t lab08_wwdg_refreshes;
extern volatile uint32_t lab08_wwdg_error;
extern volatile uint32_t lab08_wwdg_ewi;
/* One CMSIS-RTOS2 task, one run per joint reset. Static config argument.
 * No generated early MX_WWDG2_Init call, no other feeding path. */
void Lab08_WWDG_Run(void *argument);
/* Call this from the GENERATED WWDG_IRQHandler, exactly once per IRQ. */
void Lab08_WWDG_IRQHandler(void);
/* Complete GPIO definitions are in lab08_board.c. Pins labelled in CubeMX. */
void Lab08_OutputOff(void);
void Lab08_OutputOn(void);
void Lab08_MarkRefresh(void);
void Lab08_MarkEWI(void);
#endif
