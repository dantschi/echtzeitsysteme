#include "lab09_capstone.h"
/* One definition per image, same physical address. This replaces the Multicore lab07 and archived Safety shared blocks.
 * Platform contract: aligned Armv7-M words, non-cacheable shared region,
 * DMB and HSEM ownership; not a portable ISO C concurrency primitive. */
volatile Lab09_Box lab09_box
    __attribute__((section(".shared"), aligned(32), used));
