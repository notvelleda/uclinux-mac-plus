/*****************************************************************************/
/* 
 *  crt0_rom.S v1.0 <2003-07-28 17:43:30 gc>
 * 
 *  linux/arch/m68knommu/platform/68000/68Katy/crt0_rom.S
 *
 *  boot code for 68Katy board
 *  you probably need an own version of this file for your board
 *
 *  Author:     Steve Chamberlin (steve.chamberlin@gmail.com)
 *
 *  This program is free software;  you can redistribute it and/or modify it
 *  under the  terms of the GNU  General Public License as  published by the
 *  Free Software Foundation.  See the file COPYING in the main directory of
 *  this archive for more details.
 *
 *  This program  is distributed  in the  hope that it  will be  useful, but
 *  WITHOUT   ANY   WARRANTY;  without   even   the   implied  warranty   of
 *  MERCHANTABILITY  or  FITNESS FOR  A  PARTICULAR  PURPOSE.   See the  GNU
 *  General Public License for more details.
 * *
 *  Change history:
 *
 */
 /****************************************************************************/

#define M68KATY_LED 0x07E000
        
	
.global _stext
.global _start
.global _boot_stack
.global boot_entry        
        	.text
_start:
_stext:
#        movew #0, %d0
#loop:
#        movew %d0, 68KATY_LED
#        addqw #1, %d0
#        movel #50000, %d1
#loop1:  
#        dbf %d1, loop1
#        bra loop

	nop
	movew	#0x2700, %sr		/* disable interrupts: */
	moveal	#_boot_stack, %sp	/* set up stack at the end of RAM: */



#if 1
	/* Copy data segment from ROM to RAM */
	moveal	#__data_rom_start, %a0
	moveal	#__data_start, %a1
	moveal	#__data_end, %a2

	/* Copy %a0 to %a1 until %a1 == %a2 */
LD1:
	movel	%a0@+, %d0
	movel	%d0, %a1@+
	cmpal	%a1, %a2
	bhi	LD1
#endif

	moveal	#__bss_start, %a0
	moveal	#end, %a1

	/* Copy 0 to %a0 until %a0 == %a1 */
L1:
	movel	#0, %a0@+
	cmpal	%a0, %a1
	bhi	L1


	pea	0
	pea	env
	pea	%sp@(4)
	pea	0


        jsr start_kernel

_exit:
        bra	_exit

        .data
env:
	.long	0
        
