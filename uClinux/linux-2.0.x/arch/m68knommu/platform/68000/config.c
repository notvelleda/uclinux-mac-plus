/*****************************************************************************/
/* 
 *  config.c v1.0 <2003-07-28 17:55:00 gc>
 * 
 *  linux/arch/m68knommu/platform/68000/config.c
 *
 *  uClinux version 2.0.x MC68000 board initalization
 *
 *  Author:     Guido Classen (classeng@clagi.de)
 *              Steve Chamberlin (steve.chamberlin@gmail.com)
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
 *
 *  Thanks to:
 *    inital code taken from 68328/config.c
 *
 *      Copyright (C) 1993 Hamish Macdonald
 *      Copyright (C) 1999 D. Jeff Dionne
 *
 *  Change history:
 *       2002-05-15 G. Classen: initial version for MC68000
 *       2014-11-09 S. Chamberlin: cleaned up SM2010-specific code
 *
 */
 /****************************************************************************/

#include <stdarg.h>
#include <linux/config.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/console.h>

#include <asm/setup.h>
#include <asm/system.h>
#include <asm/pgtable.h>
#include <asm/irq.h>
#include <asm/machdep.h>

#ifdef CONFIG_SM2010
#include "SM2010/sm2010_hw.h"
void mpsc_console_initialize(void);
#endif

#ifdef CONFIG_68KATY
#include "68Katy/68Katy_hw.h"
void ft245_console_initialize(void);
#endif

#ifdef CONFIG_MAC_PLUS
#include "MacPlus/mac_hw.h"
void (*bsp_timer_routine)(int, void *, struct pt_regs *);
extern void mac_keyb_init(void);
extern void mac_keyb_int_handler(void);
extern void fbcon_vbl_handler(int irq, struct pt_regs *fp, void *dummy);
#endif

void config_M68000_irq(void);

#ifdef CONFIG_SM2010
/* initialize timer hardware */
static void sm2010_init_timer_hw(void)
{  
    /* Timer 0 controlwort out = low MODE 0*/  
    SM2010_TIMER.control    = 0x30;  
    SM2010_TIMER.counter0   = 0;  
    SM2010_TIMER.counter0   = 0;  
    /* Timer 2 nur controlwort out = high MODE 2*/  
    SM2010_TIMER.control    = 0xb4;
  
    /* timer2 auf 2ms initialisieren */  
    SM2010_TIMER.counter2   = ((SM2010_SIO_CLOCK_SYS / 500)) & 0xff;  
    SM2010_TIMER.counter2   = ((SM2010_SIO_CLOCK_SYS / 500)) >> 8;  
    SM2010_RESET_TIMER_INT2 = 0;  
    SM2010_TIMER.control    = 0x74;  
    SM2010_TIMER.counter1   = (SM2010_SIO_CLOCK_SYS / 1000) & 0xff;  
    SM2010_TIMER.counter1   = (SM2010_SIO_CLOCK_SYS / 1000) >> 8;
}
#endif

#ifdef CONFIG_SM2010
static void timer1_interrupt(int irq, void *dummy, struct pt_regs * regs)
{  
    SM2010_RESET_TIMER_INT1 = 0;
}
#endif

#ifdef CONFIG_MAC_PLUS

struct BootInfo { // populated by bootloader, tells us where everything is
    char *arguments;
    char *kernelPtr;
    unsigned long kernelSize;
    char *initrdPtr;
    unsigned long initrdSize;
    unsigned char hasInitrd;
};

struct BootInfo *info = (struct BootInfo *) 0x380000;

extern unsigned long initrd_start, initrd_end;
extern int mount_initrd;

extern int (*mach_keyb_init) (void);

#define VIA_TIME(ms) ((ms) * 392)

// configure via
static void mac_configure_via() {
    int timer = VIA_TIME(1000 / 200);

    MAC_VIA_IER = 0x7f; // disable all VIA interrupts (:

    // set up timer 1
    MAC_VIA_ACR &= 0x7f; // clear bit 7
    MAC_VIA_ACR |= 0x40; // set bit 6
    MAC_VIA_T1C_L = MAC_VIA_T1L_L = timer & 0xff;
    MAC_VIA_T1C_H = MAC_VIA_T1L_H = timer >> 8;

    MAC_VIA_IER = 0x80 | MAC_VIA_INT_TIMER1 | MAC_VIA_INT_VBLANK; // enable timer 1 interrupt and vblank interrupt
}

static u_char read_addr(volatile u_char *addr) {
    return *addr;
}

// VIA interrupt
static void via_interrupt(int irq, void *dev_id, struct pt_regs *regs) {
    u_char flag = MAC_VIA_IFR;
    MAC_VIA_IFR = 0xff; // clear interrupt flags

    // call interrupt handler based on interrupt flags

    if (flag & MAC_VIA_INT_KBRDY) {
        // keyboard ready interrupt, placed before everything else since keyboard timings are fucky and we want it prioritized
        mac_keyb_int_ready();
    }

    /*if (flag & MAC_VIA_INT_KBCLK) {
        // keyboard clock interrupt
        //printk("keyboard clock (%02x)\n", MAC_VIA_SHIFT_REG);
        mac_keyb_int_clk();
    }

    if (flag & MAC_VIA_INT_KBDAT) {
        // keyboard data interrupt
        //printk("keyboard data\n");
        mac_keyb_int_data();
    }*/

    if (flag & MAC_VIA_INT_TIMER1) {
        // call timer routine
        bsp_timer_routine(irq, dev_id, regs);
    }

    if (flag & MAC_VIA_INT_VBLANK) {
        // vblank interrupt
        fbcon_vbl_handler(irq, regs, dev_id);
        mac_keyb_send_queue();
    }

    /*if (flag & MAC_VIA_INT_TIMER2) {
        // timer 2 interrupt
        printk("timer 2 int\n");
    }

    if (flag & MAC_VIA_INT_1SEC) {
        // 1 second interrupt
        printk("1 second int\n");
    }*/
}

// SCC interrupt (currently disabled)
static void scc_interrupt(int irq, void *dev_id, struct pt_regs *regs) {
    u_char flags;

    // get modified interrupt vector (rr2b)
    read_addr(MAC_SCC_B_CTL_RD);
    asm volatile("nop"); // artificial delay to make sure scc stabilizes? not sure if this is too much or not enough
    MAC_SCC_B_CTL_WR = 2;
    asm volatile("nop");
    flags = 0xe & MAC_SCC_B_CTL_RD;

    // handle the interrupt
    switch (flags) {
        case 0x0a:
            printk("horizontal mouse movement\n");
            break;
        case 0x02:
            printk("vertical mouse movement\n");
            break;
        default:
            printk("unknown scc interrupt %02x\n", flags);
            break;
    }

    // reset interrupts (not sure why this doesn't work)
    read_addr(MAC_SCC_B_CTL_RD); // reset to register 0
    asm volatile("nop");

    /*MAC_SCC_B_CTL_WR = 0x38;
    asm volatile("nop");*/
    MAC_SCC_B_CTL_WR = 0x10; // reset ext/status interrupts
}

// VIA+SCC interrupt
static void via_scc_interrupt(int irq, void *dev_id, struct pt_regs *regs) {
    printk("via + scc interrupt?\n");
    //via_interrupt(irq, dev_id, regs);
    //scc_interrupt(irq, dev_id, regs);
}
#endif

static void 
BSP_sched_init(void (*timer_routine)(int, void *, struct pt_regs *))
{
#ifdef CONFIG_SM2010  
    /* initialize timer */  
    sm2010_init_timer_hw();  
    request_irq(SM2010_INT_NUM_TIMER2-VEC_SPUR,   
                timer_routine, IRQ_FLG_LOCK, "timer2", NULL);  
    request_irq(SM2010_INT_NUM_TIMER1-VEC_SPUR,   
                timer1_interrupt, IRQ_FLG_LOCK, "timer1", NULL);
  
    printk("\nMC68000 SM2010 support (C) 2002 Weiss-Electronic GmbH, "  
           "Guido Classen\n");
#endif
#ifdef CONFIG_68KATY  
    request_irq(M68KATY_INT_NUM_TIMER-VEC_SPUR,   
                timer_routine, IRQ_FLG_LOCK, "timer", NULL);  
    request_irq(M68KATY_INT_NUM_SERIAL_AND_TIMER-VEC_SPUR,   
                timer_routine, IRQ_FLG_LOCK, "timer-and-serial", NULL);
  
    printk("MC68000 68 Katy support by Big Mess o' Wires, "  
           "Steve Chamberlin\n");
#endif
#ifdef CONFIG_MAC_PLUS
    bsp_timer_routine = timer_routine; // we can't pass this in to request_irq() since the VIA handles many things

    // init timers
    mac_configure_via();

    // clear interrupt flag
    MAC_VIA_IFR = 0xff;

    // reset the SCC. ideally it would be enough to just disable interrupts, however if any interrupts are triggered before this
    // we can't clear them, and since the SCC is a black box with horrible documentation it's easiest to just reset it, which will
    // disable interrupts and clear any pending interrupts
    read_addr(MAC_SCC_B_CTL_RD); // reset to register 0
    asm volatile("nop");
    MAC_SCC_B_CTL_WR = 9; // select register 9
    asm volatile("nop");
    MAC_SCC_B_CTL_WR = 0xc0; // set d7 and d6, causing the scc to reset

    request_irq(MAC_INT_NUM_VIA - VEC_SPUR,
                via_interrupt, IRQ_FLG_LOCK, "via", NULL);
    request_irq(MAC_INT_NUM_SCC - VEC_SPUR,
                scc_interrupt, IRQ_FLG_LOCK, "scc", NULL);
    request_irq(MAC_INT_NUM_VIA_SCC - VEC_SPUR,
                via_scc_interrupt, IRQ_FLG_LOCK, "via-and-scc", NULL);

    printk("Mac Plus support by velleda\n");
#endif
}

void BSP_tick(void)
{
#ifdef CONFIG_SM2010  
    /* Reset Timer2 */  
    SM2010_RESET_TIMER_INT2 = 0;
#endif
#ifdef CONFIG_68KATY
    /* reset the timer interrupt */
#endif
}

unsigned long BSP_gettimeoffset(void)
{  
    return 0;
}

void BSP_gettod (int *yearp, int *monp, int *dayp,  
             int *hourp, int *minp, int *secp)
{
}

int BSP_hwclk(int op, struct hwclk_time *t)
{  
    if (!op) {  
            /* read */  
    } else {  
            /* write */  
    }  
    return 0;
}

int BSP_set_clock_mmss (unsigned long nowtime)
{
#if 0  
    short real_seconds = nowtime % 60, real_minutes = (nowtime / 60) % 60;
  
    tod->second1 = real_seconds / 10;  
    tod->second2 = real_seconds % 10;  
    tod->minute1 = real_minutes / 10;  
    tod->minute2 = real_minutes % 10;
#endif  
    return 0;
}

void BSP_reset (void)
{  
    cli();  
    HARD_RESET_NOW();
}



void config_BSP(char *command, int len)
{
#ifdef CONFIG_MAC_PLUS
    int i;
    u_char flags;

    // tell kernel where the initrd is, if one exists
    if (mount_initrd = info->hasInitrd) {
        initrd_start = (unsigned long) info->initrdPtr;
        initrd_end = (unsigned long) info->initrdPtr + info->initrdSize;
    } else {
        initrd_start = initrd_end = 0;
    }

    // copy command line arguments
    for (i = 0; i < len - 1; i ++)
        if (!(command[i] = info->arguments[i]))
            break;
    command[len - 1] = 0;

    mach_keyb_init = mac_keyb_init;
#endif
#ifdef CONFIG_SM2010  
    mpsc_console_initialize();
#endif
#ifdef CONFIG_FT245_SERIAL
    ft245_console_initialize();
#endif
  
    mach_sched_init      = BSP_sched_init;  
    mach_tick            = BSP_tick;  
    mach_gettimeoffset   = BSP_gettimeoffset;  
    mach_gettod          = BSP_gettod;  
    mach_hwclk           = NULL;  
    mach_set_clock_mmss  = NULL;  
    mach_mksound         = NULL;  
    mach_reset           = BSP_reset;  
    mach_debug_init      = NULL;
  
    config_M68000_irq();

#ifdef CONFIG_SM2010  
    /* enable interrupts :-) */  
    SM2010_BOARD_CONTROL = 0x1f;  
#endif
}
