#include "lab06_ipc.h"

/* Compile once into EACH image. The two linker scripts fix the SAME address.
 * No C initializer: CM7 initializes before CM4 boot release.
 * KEEP(.shared) and NOLOAD belong in both actual linker scripts. */
volatile ipc_block_t ipc
    __attribute__((section(".shared"), aligned(LAB06_CACHE_LINE), used));
