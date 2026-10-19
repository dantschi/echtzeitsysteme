#ifndef LAB07_SUMMARY_H
#define LAB07_SUMMARY_H
#include <stddef.h>
#include "lab07_timing.h"
typedef struct {
    uint32_t count, minimum, maximum, core_hz;
    uint64_t sum;
} Lab07_Summary;
int Lab07_Summarize(const volatile Lab07_Record *records, uint32_t count,
                    Lab07_Kind kind, Lab07_Summary *out); /* Student. */
extern volatile Lab07_Summary lab07_summary;
extern volatile uint32_t lab07_analysis_valid;
void Lab07_ShowAnalysis(void); /* Once after successful Run, CM7 only. */
#endif
