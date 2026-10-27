ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM   (xrw)     : ORIGIN = 0x20000000, LENGTH = 128K
    CCMRAM (xrw)    : ORIGIN = 0x10000000, LENGTH = 64K
}

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
    } > FLASH

    .text :
    {
        . = ALIGN(4);
        *(.text*)
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)
        KEEP(*(.init))
        KEEP(*(.fini))
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    .ccmram (NOLOAD) :
    {
        . = ALIGN(4);
        *(.ccmram*)
        . = ALIGN(4);
    } > CCMRAM

    .heap (NOLOAD) :
    {
        . = ALIGN(8);
        __heap_start__ = .;
        . = ORIGIN(RAM) + LENGTH(RAM) - 8;
        __heap_end__ = .;
    } > RAM

    /* Stack grows downward; leave room above heap region */
    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        _stack_top = ORIGIN(RAM) + LENGTH(RAM);
    } > RAM
}