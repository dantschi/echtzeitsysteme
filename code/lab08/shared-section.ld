/* INTEGRATION FRAGMENT, not a complete linker script.
 * In BOTH actual linker scripts, reserve SRAM4 exclusively, replacing any
 * existing alias for that range. Private .data/.bss/heap/stack go elsewhere:
 * RAM_SHARED (rw) : ORIGIN = 0x38000000, LENGTH = 64K
 * Insert the section INSIDE the existing SECTIONS block. Remove the lab06
 * section/object first; this layout REPLACES its 12-byte protocol. */
.shared ORIGIN(RAM_SHARED) (NOLOAD) :
{
    . = ALIGN(32);
    __lab08_shared_begin = .;
    KEEP(*(.shared .shared.*))
    __lab08_shared_data_end = .;
    . = ALIGN(32);
} > RAM_SHARED
ASSERT(ADDR(.shared) == 0x38000000, "Shared base differs")
ASSERT(__lab08_shared_data_end - __lab08_shared_begin == 28,
       "Only the 28-byte lab08 object may occupy shared memory")
ASSERT(SIZEOF(.shared) == 32, "Shared section must be one padded object")
ASSERT(lab08_shared == 0x38000000, "Shared symbol differs")
/* Inspect BOTH ELF/map files and the startup copy/zero tables.
 * NOLOAD alone cannot prohibit arbitrary startup/debugger writes. */
