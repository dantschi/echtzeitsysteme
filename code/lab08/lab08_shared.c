#include "lab08_counter.h"
/* One definition in EACH image, same physical address in both linker maps.
 * NOLOAD is required; startup/debugger must not zero the live shared region.
 * Aligned Armv7-M platform contract, not portable ISO C concurrency. */
volatile Lab08_Shared lab08_shared
    __attribute__((section(".shared"), aligned(32), used));
