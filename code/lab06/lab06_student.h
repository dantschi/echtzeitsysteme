#ifndef LAB06_STUDENT_H
#define LAB06_STUDENT_H
#include <stdint.h>
static inline uint32_t Lab06_ExerciseRequest(uint32_t round)
{
    static const uint32_t request[3] = {4U, 11U, 23U};
    return request[round]; /* Prepared caller bounds round to 0..2. */
}
#if defined(CORE_CM4)
void Lab06_Respond(void); /* Student publishes one response; no waiting. */
#endif
#endif
