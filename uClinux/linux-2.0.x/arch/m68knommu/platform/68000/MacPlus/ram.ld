MEMORY {
    /* interrupt vectors in RAM */
    ramvec  : ORIGIN = 0x00000000, LENGTH = 0x400

    /* user RAM memory (assumes 4 mb, starts at 0x1000 to make room for mac system variables) */
    ram     : ORIGIN = 0x00001000, LENGTH = 0x003f2000 - 0x1000

    /* end of RAM (used to set %sp) */
    eram    : ORIGIN = 0x003f2000, LENGTH = 1
}

SECTIONS {
    .ramvec : {
        _ramvec = .;
        *(.ramvec)
    } > ramvec

    .text : {
        _rambase = .;
        _stext = .;
        *(.text)
        . = ALIGN(0x10);
        _etext = ALIGN(0x10);
    } > ram

    .data : {
        _sdata = .;
        *(.rodata)
        . = ALIGN(0x4);
        *(.data)
        . = ALIGN(0x10);
        _edata = ALIGN(0x10);
    } > ram

    .bss : {
        _sbss = .;
        *(.bss)
        . = ALIGN(0x4);
        *(COMMON)
        . = ALIGN(0x10);
        _ebss = ALIGN(0x10);
        _end = ALIGN(0x10);
    } > ram

    .eram : {
        _boot_stack = . - 4;
        _ramend = .;
    } > eram
}
