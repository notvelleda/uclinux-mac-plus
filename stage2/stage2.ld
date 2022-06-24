OUTPUT_FORMAT("binary")
ENTRY(start)
phys = 0x300000;
SECTIONS {
  .text phys : AT(phys) {
    code = .;
    *(.text)
    *(.rodata)
    . = ALIGN(1024);
  }
  .data : AT(phys + (data - code)) {
    data = .;
    *(.data)
    . = ALIGN(1024);
  }
  .bss : AT(phys + (bss - code)) {
    bss = .;
    *(.bss)
    . = ALIGN(1024);
  }
  end = .;
}
