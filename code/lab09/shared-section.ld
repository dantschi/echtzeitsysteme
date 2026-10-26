/* Integration fragment, not a complete firmware linker script.
 * BOTH actual scripts reserve SRAM4 exclusively, with no overlapping alias:
 * RAM_SHARED (rw) : ORIGIN = 0x38000000, LENGTH = 64K
 * Remove the Multicore lab07 and archived Safety .shared objects and section definitions. Private data,
 * heap and stacks go elsewhere. Insert inside existing SECTIONS: */
.shared ORIGIN(RAM_SHARED) (NOLOAD) :
{
    . = ALIGN(32);
    __lab09_shared_begin = .;
    KEEP(*(.shared .shared.*))
    __lab09_shared_data_end = .;
    . = ALIGN(32);
} > RAM_SHARED
ASSERT(ADDR(.shared) == 0x38000000, "Mailbox base differs")
ASSERT(__lab09_shared_data_end - __lab09_shared_begin == 44,
       "Only the 44-byte lab09 mailbox may occupy shared memory")
ASSERT(SIZEOF(.shared) == 64, "Mailbox must be one padded object")
ASSERT(lab09_box == 0x38000000, "Mailbox symbol differs")
/* Verify both ELF/map files AND startup copy/zero tables. NOLOAD cannot
 * prohibit arbitrary startup/debugger writes into this range. */
