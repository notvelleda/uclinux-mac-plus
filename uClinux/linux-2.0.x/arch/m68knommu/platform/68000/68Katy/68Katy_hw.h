/*****************************************************************************/
/* 
 *  68Katy_hw.h
 * 
 *  linux/arch/m68knommu/platform/68000/68Katy/68Katy_hw.h
 *
 *  hardware defines for the 68Katy board
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

#ifndef __68KATY_HW_H
#define __68KATY_HW_H

#include <asm/traps.h>

#define M68KATY_INT_NUM_SERIAL   VEC_INT2
#define M68KATY_INT_NUM_TIMER    VEC_INT5
#define M68KATY_INT_NUM_SERIAL_AND_TIMER VEC_INT7

#define M68KATY_SERIAL_IN        (*(volatile u8 *)  0x078000)
#define M68KATY_SERIAL_OUT       (*(volatile u8 *)  0x07A000)
#define M68KATY_SERIAL_STATUS_RDF    (*(volatile u8 *)  0x07C000)
#define M68KATY_SERIAL_STATUS_TXE    (*(volatile u8 *)  0x07D000)
#define M68KATY_LED_OUT          (*(volatile u8 *)  0x07E000)

#endif  /* __68KATY_HW_H */

