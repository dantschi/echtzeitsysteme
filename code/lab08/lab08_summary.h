#ifndef LAB08_SUMMARY_H
#define LAB08_SUMMARY_H
#include <stddef.h>
#include "lab08_timing.h"
typedef struct {
    uint32_t count, minimum, maximum, core_hz;
    uint64_t sum;
} Lab08_Summary;
int Lab08_Summarize(const volatile Lab08_Record *records, uint32_t count,
                    Lab08_Kind kind, Lab08_Summary *out); /* Student. */
extern volatile Lab08_Summary lab08_summary;
extern volatile uint32_t lab08_analysis_valid;
void Lab08_ShowAnalysis(void); /* Once after successful Run, CM7 only. */
#endif
