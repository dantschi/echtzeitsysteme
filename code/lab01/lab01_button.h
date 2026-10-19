#ifndef LAB01_BUTTON_H
#define LAB01_BUTTON_H
#include "main.h"
#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "Button exercise requires CM4"
#endif
void Lab01_ButtonInit(void);
void Lab01_ButtonRunOnce(void);
int Lab01_ButtonPressed(void);
void Lab01_ButtonLed(int on);
void Lab01_ButtonStep(void); /* Student implementation. */
extern volatile uint32_t lab01_presses; /* Main only; wraps modulo 2^32. */
#endif
