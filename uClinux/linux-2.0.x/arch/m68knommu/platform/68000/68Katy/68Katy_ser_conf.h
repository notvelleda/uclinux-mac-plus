/*****************************************************************************/
/* 
 *  68Katy_ser_conf.h
 * 
 *  linux/arch/m68knommu/platform/68000/68Katy/68Katy_ser_conf.h
 *
 *  hardware configuration for FT245 serial driver for the 68Katy board
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
 *       2014-11-10 sc: initial version
 *
 */
 /****************************************************************************/

#ifndef __68KATY_SER_CONF_H
#define __68KATY_SER_CONF_H

#include "68Katy_hw.h"

static const ft245_port ft245_port_info[] = {
	{ &M68KATY_SERIAL_OUT, 
	  &M68KATY_SERIAL_IN, 
	  &M68KATY_SERIAL_STATUS_RDF,
	  &M68KATY_SERIAL_STATUS_TXE,
	  M68KATY_INT_NUM_SERIAL }
};

#define NUM_PORTS 1
#define CONSOLE_PORT 0

#endif

