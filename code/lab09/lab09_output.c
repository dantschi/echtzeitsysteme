#include "main.h"
#include "lab09_capstone.h"
#if !defined(CORE_CM7) || defined(CORE_CM4)
#error "Output adapter belongs only to CM7"
#endif
#if defined(LAB09_UART_HANDLE)
#include <stdio.h>
extern UART_HandleTypeDef LAB09_UART_HANDLE;
#endif
int Lab09_CM7_Output(const Lab09_Result *result)
{
#if defined(LAB09_UART_HANDLE)
    char line[80];
    int size = snprintf(line, sizeof line, "e=%lu p=%lu x=%lu y=%lu\r\n",
        (unsigned long)result->record.event_seq, (unsigned long)result->record.pub_seq,
        (unsigned long)result->record.sample, (unsigned long)result->filtered);
    if (size <= 0 || (size_t)size >= sizeof line) { return 0; }
    return HAL_UART_Transmit(&LAB09_UART_HANDLE, (uint8_t *)line,
                            (uint16_t)size, 20u) == HAL_OK;
#else
    (void)result;
    return 1; /* Results already stored in private RAM, bounded to 20 records. */
#endif
}
