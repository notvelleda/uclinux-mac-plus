MEMORY {
    /* interrupt vectors in RAM */
    ramvec  : ORIGIN = 0x00000000, LENGTH = 0x400

    /* user accessible RAM (maximum of 4 MB, starts at 0x2000 to make room for Mac system variables) */
    ram     : ORIGIN = 0x00002000, LENGTH = 0x00400000 - 0x2000

    /* end of RAM (used to set %sp) */
    eram    : ORIGIN = 0x003f2000, LENGTH = 1
}

SECTIONS {
    .ramvec (NOLOAD) : {
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
        _ramend = .;
    } > eram
}
