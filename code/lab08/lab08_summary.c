#include "main.h"
#include "lab08_summary.h"
#if !defined(LAB08_RESULT_Pin) || !defined(LAB08_RESULT_GPIO_Port)
#error "Configure onboard LD1 as LAB08_RESULT on CM7"
#endif
volatile Lab08_Summary lab08_summary;
volatile uint32_t lab08_analysis_valid;
void Lab08_ShowAnalysis(void)
{
    Lab08_Summary summary;
    if (lab08_done != 0U && lab08_status == LAB08_OK &&
        Lab08_Summarize(lab08_records, lab08_count, LAB08_WORK, &summary)) {
        lab08_summary = summary;
        lab08_analysis_valid = 1U;
    } else { lab08_analysis_valid = 0U; }
    HAL_GPIO_WritePin(LAB08_RESULT_GPIO_Port, LAB08_RESULT_Pin,
                      lab08_analysis_valid ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
