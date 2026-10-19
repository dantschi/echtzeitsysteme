#ifndef LAB08_HEALTH_H
#define LAB08_HEALTH_H
#include <stddef.h>
#include <stdint.h>
#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "Health exercise requires CM4"
#endif
typedef struct {
    uint32_t now, last_a, last_b;
    int have_a, have_b;
} Lab08_Health;
void Lab08_HealthStart(void); /* Once in kernel Ready. */
int Lab08_Healthy(const Lab08_Health *snapshot); /* Student; private input. */
extern volatile Lab08_Health lab08_health_snapshot; /* Supervisor-only writer. */
extern volatile uint32_t lab08_health_valid; /* Halt CM4 before inspection. */
#endif
