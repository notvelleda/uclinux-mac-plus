/*****************************************************************************/
/* 
 *  mac_hw.h
 * 
 *  linux/arch/m68knommu/platform/68000/MacPlus/mac_hw.h
 *
 *  hardware defines for the Mac Plus
 *
 *  Author: velleda
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

#ifndef __MAC_HW_H
#define __MAC_HW_H

#include <asm/traps.h>

#define MAC_INT_NUM_VIA     VEC_INT1
#define MAC_INT_NUM_SCC     VEC_INT2
#define MAC_INT_NUM_VIA_SCC VEC_INT3

// VIA base address
#define MAC_VIA_BASE        0xefe1fe

// VIA register addresses
#define MAC_VIA_BUFB        (*((volatile u_char *) (MAC_VIA_BASE + 512 * 0)))   // register B (zero offset)
#define MAC_VIA_DDRB        (*((volatile u_char *) (MAC_VIA_BASE + 512 * 2)))   // register B direction register
#define MAC_VIA_DDRA        (*((volatile u_char *) (MAC_VIA_BASE + 512 * 3)))   // register A direction register
#define MAC_VIA_T1C_L       (*((volatile u_char *) (MAC_VIA_BASE + 512 * 4)))   // timer 1 counter (low-order byte)
#define MAC_VIA_T1C_H       (*((volatile u_char *) (MAC_VIA_BASE + 512 * 5)))   // timer 1 counter (high-order byte)
#define MAC_VIA_T1L_L       (*((volatile u_char *) (MAC_VIA_BASE + 512 * 6)))   // timer 1 latch (low-order byte)
#define MAC_VIA_T1L_H       (*((volatile u_char *) (MAC_VIA_BASE + 512 * 7)))   // timer 1 latch (high-order byte)
#define MAC_VIA_T2C_L       (*((volatile u_char *) (MAC_VIA_BASE + 512 * 8)))   // timer 2 counter (low-order byte)
#define MAC_VIA_T2C_H       (*((volatile u_char *) (MAC_VIA_BASE + 512 * 9)))   // timer 2 counter (high-order byte)
#define MAC_VIA_SR          (*((volatile u_char *) (MAC_VIA_BASE + 512 * 10)))  // shift register (keyboard)
#define MAC_VIA_ACR         (*((volatile u_char *) (MAC_VIA_BASE + 512 * 11)))  // auxiliary control register
#define MAC_VIA_PCR         (*((volatile u_char *) (MAC_VIA_BASE + 512 * 12)))  // peripheral control register
#define MAC_VIA_IFR         (*((volatile u_char *) (MAC_VIA_BASE + 512 * 13)))  // interrupt flag register
#define MAC_VIA_IER         (*((volatile u_char *) (MAC_VIA_BASE + 512 * 14)))  // interrupt enable register
#define MAC_VIA_BUFA        (*((volatile u_char *) (MAC_VIA_BASE + 512 * 15)))  // register A

// VIA register A constants
#define MAC_VIA_REG_A_OUT   0x7f    // direction register A:  1 bits = outputs
#define MAC_VIA_REG_A_INIT  0x7b    // initial value for MAC_VIA_BUF_A (medium volume)
#define MAC_VIA_VOL_SOUND   0x7     // sound volume bits

// VIA register A bit numbers
#define MAC_VIA_SOUND_PG2   0x3     // 0 = alternate sound buffer
#define MAC_VIA_ROM_OVERLAY 0x4     // 1 = ROM overlay (system startup only)
#define MAC_VIA_HEAD_SEL    0x5     // disk SEL control line
#define MAC_VIA_SCREEN_PG2  0x6     // 0 = alternate screen buffer
#define MAC_VIA_SCC_WREQ    0x7     // SCC wait/request line

// VIA register B constants
#define MAC_VIA_REG_B_OUT   0x87    // direction register B:  1 bits = outputs
#define MAC_VIA_REG_B_INIT  0x07    // initial value for MAC_VIA_BUF_B

// VIA interrupt bits
#define MAC_VIA_INT_IRQ     0x80    // IRQ (all enabled VIA interrupts)
#define MAC_VIA_INT_TIMER1  0x40    // Timer 1
#define MAC_VIA_INT_TIMER2  0x20    // Timer 2
#define MAC_VIA_INT_KBCLK   0x10    // Keyboard clock
#define MAC_VIA_INT_KBDAT   0x8     // Keyboard data bit
#define MAC_VIA_INT_KBRDY   0x4     // Keyboard data ready
#define MAC_VIA_INT_VBLANK  0x2     // Vertical blanking interrupt
#define MAC_VIA_INT_1SEC    0x1     // One-second interrupt

// keyboard commands
#define MAC_KEYB_INQ        0x10    // Inquiry
#define MAC_KEYB_INST       0x14    // Instant
#define MAC_KEYB_MODEL      0x16    // Model Number
#define MAC_KEYB_TEST       0x36    // Test

// null response
#define MAC_KEYB_NULL       0x7b
#define MAC_KEYB_ACK        0x7d
#define MAC_KEYB_NACK       0x77

#if 0
// SCSI
#define MAC_SCSI_BASE       0x580000

#define MAC_SCSI_READ       (MAC_SCSI_BASE + 0x0)
#define MAC_SCSI_WRITE      (MAC_SCSI_BASE + 0x1)

#define NCR5380_local_declare()

#define NCR5380_write(r, v) *((volatile u_char *) (MAC_SCSI_WRITE + (r << 4))) = (u_char) (v)

#define NCR5380_read(r) *((volatile u_char *) (MAC_SCSI_READ + (r << 4)))

#define NCR5380_implementation_fields
#endif

#endif /* __MAC_HW_H */
