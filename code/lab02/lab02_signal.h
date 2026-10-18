#ifndef LAB02_SIGNAL_H
#define LAB02_SIGNAL_H

#include "lab02_demo.h"

/* CubeMX provides LAB_MARKER on the signal LED and LAB_STATUS on a second
   LED. CM4 owns both. Call once after HAL/GPIO/dual-core boot setup. */
int Lab02_SignalInit(void);

/* Implement this bounded callback in the student's signal_step.c. */
void Lab02_SignalStep(void);

void Lab02_SignalWrite(int on);
/* Sets the active callback's wake metadata; the callback must return. */
void Lab02_SignalPause(uint32_t ms);

/* Written only by a bounded background callback. Read with CM4 halted.
   Wraps modulo 2^32; not a timing measurement. */
extern volatile uint32_t lab02_signal_checks;

#endif
