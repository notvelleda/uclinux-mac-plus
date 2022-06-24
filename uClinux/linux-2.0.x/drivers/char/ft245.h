/*****************************************************************************/
/* 
 *  ft245.h
 * 
 *  linux/drivers/char/ft245.h
 *
 *  Author:     Steve Chamberlin (steve.chamberlin@gmail.com)
 *
 *  This program is free software;  you can redistribute it and/or modify it
 *  under the  terms of the GNU  General Public License as  published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
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

#ifndef __FT245_H
#define __FT245_H

#define FT245_WRITE_DATA(scc, dat) ( *(scc).output = (dat) )
#define FT245_READ_DATA(scc) ( *(scc).input) 
#define FT245_READ_STATUS_RDF(scc) ( *(scc).status_rdf) 
#define FT245_READ_STATUS_TXE(scc) ( *(scc).status_txe) 

#define FT245_STATUS_BIT 1

typedef struct _ft245_port {
        volatile u8 *output;
        volatile u8 *input;
        volatile u8 *status_rdf;
        volatile u8 *status_txe;
        int vecnum;
} ft245_port;

/* forward declaration */
struct tty_struct;

typedef struct _ft245_info {
    struct _ft245_port portaddrs;
    /* per-port runtime info goes here */
    int magic;
    int line;
    int irq;
    struct tty_struct *tty;
} ft245_info;

#endif /* __FT245_H */
