/* Add only to CM4, with LAB07_STUDENT_EXERCISE in BOTH images.
   The empty starting point intentionally fails the post-call phase check. */
#include "lab07_ipc.h"
#include "lab07_student.h"
#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "Response exercise requires CM4"
#endif
void Lab07_Respond(void) {}
