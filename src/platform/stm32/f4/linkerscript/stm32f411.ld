MEMORY
{
    FLASH (RX)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (RW) : ORIGIN = 0x20000000,   LENGTH = 128K
}

SECTIONS
{
    .isr_vector :
    {
        /* The ISR vector has to avoid the linker garbage collector */
        KEEP(*(.isr_vector));
    } > FLASH :text

    .text :
    {
        *(.text);
        *(.text*);
    } > FLASH :text

    .rodata :
    {
        *(.rodata);
        *(.rodata*);
    } > FLASH :text

    .bss (NOLOAD) :
    {
        *(.bss);
        *(.bss*);
    } > RAM

    .data :
    {
        *(.data);
        *(.data*);
    } > RAM AT> FLASH :data
}

PHDRS
{
    text PT_LOAD FLAGS(rx);
    data PT_LOAD FLAGS(rw);
}

__text_start = ADDR(.text);
__text_end = __text_start + SIZEOF(.text);

__rodata_start = ADDR(.rodata);
__rodata_end = __rodata_start + SIZEOF(.rodata);

__bss_start = ADDR(.bss);
__bss_end = __bss_start + SIZEOF(.bss);

__rom_data_start = LOADADDR(.data);
__rom_data_end = __rom_data_start + SIZEOF(.data);
__data_start = ADDR(.data);
__data_end = __data_start + SIZEOF(.data);

/* The stack grows down it can be defined from the end of the RAM */
__stack_start = ORIGIN(RAM) + LENGTH(RAM);

