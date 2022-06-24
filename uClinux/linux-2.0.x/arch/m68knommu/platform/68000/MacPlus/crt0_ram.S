.global _start
.global _boot_stack
.global boot_entry        


    .text

_start:
    movew #0x2700, %sr /* disable interrupts: */
    moveal #_boot_stack, %sp /* set up stack at the end of RAM: */

    moveal #_sbss, %a0
    moveal #_end, %a1

    /* Copy 0 to %a0 until %a0 == %a1 */
L1:
    movel #0, %a0@+
    cmpal %a0, %a1
    bhi L1

    pea 0
    pea env
    pea %sp@(4)
    pea 0

    jsr start_kernel

_exit:
    bra _exit


    .data

env:
    .long    0
