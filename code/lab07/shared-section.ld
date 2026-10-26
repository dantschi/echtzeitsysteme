/* INTEGRATION FRAGMENT, not a complete firmware linker script.
 * Both images must use their ACTUAL linker script selected by CMake.
 * In MEMORY, reserve SRAM4 once. Replace an existing alias for this span:
 *
 * RAM_SHARED (rw) : ORIGIN = 0x38000000, LENGTH = 64K
 *
 * No .bss, .data, heap, stack or other private allocation in RAM_SHARED.
 * Insert this output section INSIDE the existing SECTIONS block: */
.shared ORIGIN(RAM_SHARED) (NOLOAD) :
{
    . = ALIGN(32);
    __lab07_shared_begin = .;
    KEEP(*(.shared .shared.*))
    __lab07_shared_data_end = .;
    . = ALIGN(32);
} > RAM_SHARED

ASSERT(ADDR(.shared) == 0x38000000, "IPC base address differs")
ASSERT(__lab07_shared_data_end - __lab07_shared_begin == 12,
       "Only the 12-byte IPC object may occupy the shared section")
ASSERT(SIZEOF(.shared) == 32, "IPC section must contain one padded object")
ASSERT(ipc == 0x38000000, "IPC symbol differs")

/* Also inspect the startup copy/zero tables and both actual ELF/map files.
 * NOLOAD does not itself prevent arbitrary startup writes or debugger writes. */
