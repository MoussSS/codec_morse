ENTRY(main)

MEMORY
{
    RAM(xrw): ORIGIN = 0x20000000, LENGTH = 128K
    ROM(rw) : ORIGIN = 0x08000000, LENGTH = 512K
}

STACK_SIZE = 0x400;

/* Section Definitions */
SECTIONS
{
    .text :
    {
        KEEP(*(.vectors .vectors.*))
        *(.text*)
        *(.rodata*)
    } > ROM

    /* .bss section which is used for uninitialized data */
    .bss (NOLOAD) :
    {
        *(.bss*)
        *(COMMON)
    } > RAM

    .data :
    {
        *(.data*);
    } > RAM AT >ROM

    /* stack section */
    .stack (NOLOAD):
    {
        . = ALIGN(8);
        . = . + STACK_SIZE;
        . = ALIGN(8);
    } > RAM

    _end = . ;
}