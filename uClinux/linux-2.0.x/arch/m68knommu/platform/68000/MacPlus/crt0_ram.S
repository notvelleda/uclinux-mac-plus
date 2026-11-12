.global _start
.global _boot_stack
.global boot_entry        
.global command_line_from_bootloader

    .text

.equ MemTop, 0x0108             /* Mac low memory global: address of the end of RAM */
.equ VideoSoundArea, 0xe000     /* screen + sound buffers at the end of RAM. See def of memory_end in uClinux/linux-2.0.x/arch/m68knommu/kernel/setup.c */

_start:
    /* interrupts should already be disabled and bss should be cleared by the bootloader */

    movew #0x2700, %sr
    movel 12(%sp), command_line_from_bootloader

    /* Move the stack below the video and sound buffers as it is not yet set at the end of RAM */
    movel (MemTop), %d0
    subl #VideoSoundArea + 4, %d0
    movel %d0, %sp

    jsr start_kernel

_exit:
    bra _exit


    .data

command_line_from_bootloader:
    .long    0
