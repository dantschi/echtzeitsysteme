#ifndef LAB01_BUTTON_H
#define LAB01_BUTTON_H
#include "main.h"
#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "Button exercise requires CM4"
#endif
/* Prepared setup: once after GPIO/HAL/dual-core boot, main context only.
   A running 1-kHz HAL tick and single CM4 ownership of both LEDs are required. */
#define LAB01_LIGHT_TICKS UINT32_C(800)
void Lab01_ButtonInit(void);
void Lab01_ButtonRunOnce(void);
/* Initial raw state is adopted at Init; subsequent changes require 20 ms
   stability as observed by repeated main-loop calls. Returns zero or one. */
int Lab01_ButtonPressed(void);
void Lab01_ButtonLed(int on);
/* Student implementation: bounded work; retained state across calls.
   A new debounced press lights LD1 for LAB01_LIGHT_TICKS and increments
   lab01_presses. Holding does not retrigger; a held startup counts once. */
void Lab01_ButtonStep(void);
extern volatile uint32_t lab01_presses; /* Main only; wraps modulo 2^32. */
#endif
