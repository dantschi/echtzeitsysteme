#ifndef LAB04_ACCOUNT_H
#define LAB04_ACCOUNT_H
#include <stdint.h>
#include "cmsis_os2.h"
#if !defined(CORE_CM4) || defined(CORE_CM7)
#error "Account exercise requires CM4"
#endif
typedef struct { int32_t balance, check; uint32_t done; } Lab04_Account;
/* Shared only by threads that hold this mutex; not an ISR interface. */
extern osMutexId_t lab04_account_mutex;
extern Lab04_Account lab04_account;
void Lab04_Check(osStatus_t status);
void Lab04_AccountStart(void); /* Once in kernel Ready, before Start. */
void Lab04_Adjust(int32_t delta); /* Student. */
Lab04_Account Lab04_Snapshot(void); /* Student. */
#endif
