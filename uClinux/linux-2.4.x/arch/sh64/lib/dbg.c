/*--------------------------------------------------------------------------
--
-- Identity : Linux50 Debug Funcions
--
-- File     : arch/sh64/lib/dbg.C
--
-- Copyright 2000, 2001 STMicroelectronics Limited.
--
--------------------------------------------------------------------------*/
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/mmu_context.h>

typedef u64 regType_t;

static regType_t getConfigReg(u64 id)
{
	register u64 reg __asm__("r2");
	asm volatile ("getcfg   %1, 0, %0":"=r" (reg):"r"(id));
	return (reg);
}

/* ======================================================================= */

static char *szTab[] = { "4k", "64k", "1M", "512M" };
static char *protTab[] = { "----",
	"---R",
	"--X-",
	"--XR",
	"-W--",
	"-W-R",
	"-WX-",
	"-WXR",
	"U---",
	"U--R",
	"U-X-",
	"U-XR",
	"UW--",
	"UW-R",
	"UWX-",
	"UWXR"
};
#define  ITLB_BASE	0x00000000
#define  DTLB_BASE	0x00800000
#define  MAX_TLBs		64
/* PTE High */
#define  GET_VALID(pte)        ((pte) & 0x1)
#define  GET_SHARED(pte)       ((pte) & 0x2)
#define  GET_ASID(pte)         ((pte >> 2) & 0x0ff)
#define  GET_EPN(pte)          ((pte) & 0xfffff000)

/* PTE Low */
#define  GET_CBEHAVIOR(pte)    ((pte) & 0x3)
#define  GET_PAGE_SIZE(pte)    szTab[((pte >> 3) & 0x3)]
#define  GET_PROTECTION(pte)   protTab[((pte >> 6) & 0xf)]
#define  GET_PPN(pte)          ((pte) & 0xfffff000)

#define PAGE_1K_MASK           0x00000000
#define PAGE_4K_MASK           0x00000010
#define PAGE_64K_MASK          0x00000080
#define MMU_PAGESIZE_MASK      (PAGE_64K_MASK | PAGE_4K_MASK)
#define PAGE_1MB_MASK          MMU_PAGESIZE_MASK
#define PAGE_1K                (1024)
#define PAGE_4K                (1024 * 4)
#define PAGE_64K               (1024 * 64)
#define PAGE_1MB               (1024 * 1024)

#define HOW_TO_READ_TLB_CONTENT  \
       "[ ID]  PPN         EPN        ASID  Share  CB  P.Size   PROT.\n"

void print_single_tlb(unsigned long tlb, int single_print)
{
	regType_t pteH;
	regType_t pteL;
	unsigned int valid, shared, asid, epn, cb, ppn;
	char *pSize;
	char *pProt;

	/*
	   ** in case of single print <single_print> is true, this implies:
	   **   1) print the TLB in any case also if NOT VALID
	   **   2) print out the header
	 */

	pteH = getConfigReg(tlb);
	valid = GET_VALID(pteH);
	if (single_print)
		printk(HOW_TO_READ_TLB_CONTENT);
	else if (!valid)
		return;

	pteL = getConfigReg(tlb + 1);

	shared = GET_SHARED(pteH);
	asid = GET_ASID(pteH);
	epn = GET_EPN(pteH);
	cb = GET_CBEHAVIOR(pteL);
	pSize = GET_PAGE_SIZE(pteL);
	pProt = GET_PROTECTION(pteL);
	ppn = GET_PPN(pteL);
	printk("[%c%2ld]  0x%08x  0x%08x  %03d   %02x    %02x   %4s    %s\n",
	       ((valid) ? ' ' : 'u'), ((tlb & 0x0ffff) / TLB_STEP),
	       ppn, epn, asid, shared, cb, pSize, pProt);
}

void print_dtlb(void)
{
	int count;
	unsigned long tlb;

	printk(" ================= SH-5 D-TLBs Status ===================\n");
	printk(HOW_TO_READ_TLB_CONTENT);
	tlb = DTLB_BASE;
	for (count = 0; count < MAX_TLBs; count++, tlb += TLB_STEP)
		print_single_tlb(tlb, 0);
	printk
	    (" =============================================================\n");
}

void print_itlb(void)
{
	int count;
	unsigned long tlb;

	printk(" ================= SH-5 I-TLBs Status ===================\n");
	printk(HOW_TO_READ_TLB_CONTENT);
	tlb = ITLB_BASE;
	for (count = 0; count < MAX_TLBs; count++, tlb += TLB_STEP)
		print_single_tlb(tlb, 0);
	printk
	    (" =============================================================\n");
}

/* ======================================================================= */

#include "syscalltab.h"

void evt_debug(int evt, int ret_addr, int event, int tra)
{
	int syscallno = tra & 0xff;

	// if (event != 0) printk("<%03x>",evt);

	if ((event == 2) && (evt == 0x160)) {
		if (syscallno < NUM_SYSCALL_INFO_ENTRIES)
			printk("Task %d: %s()\n",
			       current->pid,
			       syscall_info_table[syscallno].name);
	}
}

void evt_debug2(unsigned int ret)
{
	printk("Task %d: syscall returns %08x\n", current->pid, ret);
}

/* ======================================================================= */

void show_excp_regs(char *from, int trapnr, int signr, struct pt_regs *regs)
{

	unsigned long long ah, al, bh, bl, ch, cl;

	printk("\n");
	printk("EXCEPTION - %s: task %d; Linux trap # %d; signal = %d\n",
	       ((from) ? from : "???"), current->pid, trapnr, signr);

	asm volatile ("getcon   " __c5 ", %0":"=r"(ah));
	asm volatile ("getcon   " __c5 ", %0":"=r"(al));
	ah = (ah) >> 32;
	al = (al) & 0xffffffff;
	asm volatile ("getcon   " __c18 ", %0":"=r"(bh));
	asm volatile ("getcon   " __c18 ", %0":"=r"(bl));
	bh = (bh) >> 32;
	bl = (bl) & 0xffffffff;
	asm volatile ("getcon   " __c4 ", %0":"=r"(ch));
	asm volatile ("getcon   " __c4 ", %0":"=r"(cl));
	ch = (ch) >> 32;
	cl = (cl) & 0xffffffff;
	printk("EXPE: %08Lx%08Lx KCR1: %08Lx%08Lx INTE: %08Lx%08Lx\n",
	       ah, al, bh, bl, ch, cl);

	asm volatile ("getcon   " __c6 ", %0":"=r"(ah));
	asm volatile ("getcon   " __c6 ", %0":"=r"(al));
	ah = (ah) >> 32;
	al = (al) & 0xffffffff;
	asm volatile ("getcon   " __c9 ", %0":"=r"(bh));
	asm volatile ("getcon   " __c9 ", %0":"=r"(bl));
	bh = (bh) >> 32;
	bl = (bl) & 0xffffffff;
	asm volatile ("getcon   " __c2 ", %0":"=r"(ch));
	asm volatile ("getcon   " __c2 ", %0":"=r"(cl));
	ch = (ch) >> 32;
	cl = (cl) & 0xffffffff;
	printk("PEXP: %08Lx%08Lx PSPC: %08Lx%08Lx PSSR: %08Lx%08Lx\n",
	       ah, al, bh, bl, ch, cl);

	ah = (regs->pc) >> 32;
	al = (regs->pc) & 0xffffffff;
	bh = (regs->regs[18]) >> 32;
	bl = (regs->regs[18]) & 0xffffffff;
	ch = (regs->regs[15]) >> 32;
	cl = (regs->regs[15]) & 0xffffffff;
	printk("PC  : %08Lx%08Lx LINK: %08Lx%08Lx SP  : %08Lx%08Lx\n",
	       ah, al, bh, bl, ch, cl);

	ah = (regs->sr) >> 32;
	al = (regs->sr) & 0xffffffff;
	asm volatile ("getcon   " __c13 ", %0":"=r"(bh));
	asm volatile ("getcon   " __c13 ", %0":"=r"(bl));
	bh = (bh) >> 32;
	bl = (bl) & 0xffffffff;
	asm volatile ("getcon   " __c17 ", %0":"=r"(ch));
	asm volatile ("getcon   " __c17 ", %0":"=r"(cl));
	ch = (ch) >> 32;
	cl = (cl) & 0xffffffff;
	printk("SR  : %08Lx%08Lx TEA : %08Lx%08Lx KCR0: %08Lx%08Lx\n",
	       ah, al, bh, bl, ch, cl);

	ah = (regs->regs[0]) >> 32;
	al = (regs->regs[0]) & 0xffffffff;
	bh = (regs->regs[1]) >> 32;
	bl = (regs->regs[1]) & 0xffffffff;
	ch = (regs->regs[2]) >> 32;
	cl = (regs->regs[2]) & 0xffffffff;
	printk("R0  : %08Lx%08Lx R1  : %08Lx%08Lx R2  : %08Lx%08Lx\n",
	       ah, al, bh, bl, ch, cl);

	ah = (regs->regs[3]) >> 32;
	al = (regs->regs[3]) & 0xffffffff;
	bh = (regs->regs[4]) >> 32;
	bl = (regs->regs[4]) & 0xffffffff;
	ch = (regs->regs[5]) >> 32;
	cl = (regs->regs[5]) & 0xffffffff;
	printk("R3  : %08Lx%08Lx R4  : %08Lx%08Lx R5  : %08Lx%08Lx\n",
	       ah, al, bh, bl, ch, cl);

	ah = (regs->regs[6]) >> 32;
	al = (regs->regs[6]) & 0xffffffff;
	bh = (regs->regs[7]) >> 32;
	bl = (regs->regs[7]) & 0xffffffff;
	ch = (regs->regs[8]) >> 32;
	cl = (regs->regs[8]) & 0xffffffff;
	printk("R6  : %08Lx%08Lx R7  : %08Lx%08Lx R8  : %08Lx%08Lx\n",
	       ah, al, bh, bl, ch, cl);

	ah = (regs->regs[9]) >> 32;
	al = (regs->regs[9]) & 0xffffffff;
	bh = (regs->regs[10]) >> 32;
	bl = (regs->regs[10]) & 0xffffffff;
	ch = (regs->regs[11]) >> 32;
	cl = (regs->regs[11]) & 0xffffffff;
	printk("R9  : %08Lx%08Lx R10 : %08Lx%08Lx R11 : %08Lx%08Lx\n",
	       ah, al, bh, bl, ch, cl);
	printk("....\n");

	ah = (regs->tregs[0]) >> 32;
	al = (regs->tregs[0]) & 0xffffffff;
	bh = (regs->tregs[1]) >> 32;
	bl = (regs->tregs[1]) & 0xffffffff;
	ch = (regs->tregs[2]) >> 32;
	cl = (regs->tregs[2]) & 0xffffffff;
	printk("T0  : %08Lx%08Lx T1  : %08Lx%08Lx T2  : %08Lx%08Lx\n",
	       ah, al, bh, bl, ch, cl);
	printk("....\n");

	print_dtlb();
	print_itlb();
}

/* ======================================================================= */

/*
** Depending on <base> scan the MMU, Data or Instrction side
** looking for a valid mapping matching Eaddr & asid.
** Return -1 if not found or the TLB id entry otherwise.
** Note: it works only for 4k pages!
*/
static unsigned long
lookup_mmu_side(unsigned long base, unsigned long Eaddr, unsigned long asid)
{
	regType_t pteH;
	unsigned long epn;
	int count;

	epn = Eaddr & 0xfffff000;

	for (count = 0; count < MAX_TLBs; count++, base += TLB_STEP) {
		pteH = getConfigReg(base);
		if (GET_VALID(pteH))
			if ((unsigned long) GET_EPN(pteH) == epn)
				if ((unsigned long) GET_ASID(pteH) == asid)
					break;
	}
	return ((unsigned long) ((count < MAX_TLBs) ? base : -1));
}

unsigned long lookup_dtlb(unsigned long Eaddr)
{
	unsigned long asid = get_asid();
	return (lookup_mmu_side((u64) DTLB_BASE, Eaddr, asid));
}

unsigned long lookup_itlb(unsigned long Eaddr)
{
	unsigned long asid = get_asid();
	return (lookup_mmu_side((u64) ITLB_BASE, Eaddr, asid));
}

void print_page(struct page *page)
{
	printk("  page[%p] -> index 0x%lx,  count 0x%x,  flags 0x%lx\n",
	       page, page->index, atomic_read(&page->count), page->flags);
	printk("       address_space = %p, pages =%ld\n", page->mapping,
	       page->mapping->nrpages);

}