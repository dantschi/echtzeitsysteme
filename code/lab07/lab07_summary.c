#include "main.h"
#include "lab07_summary.h"
#if !defined(LAB07_RESULT_Pin) || !defined(LAB07_RESULT_GPIO_Port)
#error "Configure onboard LD1 as LAB07_RESULT on CM7"
#endif
volatile Lab07_Summary lab07_summary;
volatile uint32_t lab07_analysis_valid;
void Lab07_ShowAnalysis(void)
{
    Lab07_Summary summary;
    if (lab07_done != 0U && lab07_status == LAB07_OK &&
        Lab07_Summarize(lab07_records, lab07_count, LAB07_WORK, &summary)) {
        lab07_summary = summary;
        lab07_analysis_valid = 1U;
    } else { lab07_analysis_valid = 0U; }
    HAL_GPIO_WritePin(LAB07_RESULT_GPIO_Port, LAB07_RESULT_Pin,
                      lab07_analysis_valid ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
