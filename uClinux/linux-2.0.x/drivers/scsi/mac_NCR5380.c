/*
 * Mac Plus SCSI driver, based on basically every other NCR5380 driver here
 * (c) 2022 velleda
 */

#define AUTOSENSE
#define DONT_USE_INTR
#define PSEUDO_DMA

#ifdef MODULE
#include <linux/module.h>
#endif

#include <linux/signal.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/blk.h>
#include "scsi.h"
#include "hosts.h"
#include "mac_NCR5380.h"
#include "constants.h"
#include "sd.h"

#include <linux/stat.h>

struct proc_dir_entry proc_scsi_mac = {
    PROC_SCSI_GENERIC_NCR5380, 9, "mac",
    S_IFDIR | S_IRUGO | S_IXUGO, 2
};

/* 
 * Function : int mac_scsi_detect(Scsi_Host_Template * tpnt)
 *
 * Purpose : sets up built in NCR5380
 *
 * Inputs : tpnt - template for this SCSI adapter.
 * 
 * Returns : 1 if a host adapter was found, 0 if not.
 *
 */

int mac_scsi_detect(Scsi_Host_Template * tpnt) {
    struct Scsi_Host *instance;

    tpnt->proc_dir = &proc_scsi_mac;

    instance = scsi_register(tpnt, sizeof(struct NCR5380_hostdata));

    NCR5380_init(instance, 0);

    instance->irq = IRQ_NONE;

    printk("scsi%d : options CAN_QUEUE=%d  CMD_PER_LUN=%d", instance->host_no, CAN_QUEUE, CMD_PER_LUN);
    NCR5380_print_options(instance);
    printk("\n");

    return 1;
}

/*
 * Function : int mac_scsi_biosparam(Disk *disk, kdev_t dev, int *ip)
 *
 * Purpose : Generates a BIOS / DOS compatible H-C-S mapping for 
 *    the specified device / size.
 * 
 * Inputs : size = size of device in sectors (512 bytes), dev = block device
 *    major / minor, ip[] = {heads, sectors, cylinders}  
 *
 * Returns : always 0 (success), initializes ip
 *    
 */

/* 
 * XXX Most SCSI boards use this mapping, I could be incorrect.  Some one
 * using hard disks on a trantor should verify that this mapping corresponds
 * to that used by the BIOS / ASPI driver by running the linux fdisk program
 * and matching the H_C_S coordinates to what DOS uses.
 */

int mac_scsi_biosparam(Disk * disk, kdev_t dev, int * ip) {
    int size = disk->capacity;
    ip[0] = 64;
    ip[1] = 32;
    ip[2] = size >> 11;        /* I think I have it as /(32*64) */
    if (ip[2] > 1024) {        /* yes, >, not >= */
        ip[0] = 255;
        ip[1] = 63;
        ip[2] = size / (63 * 255);
        if (ip[2] > 1023)    /* yes >1023... */
            ip[2] = 1023;
    }

    return 0;
}

/* very basic pseudo dma- doesn't check if anything breaks */

/*
 * Function : int NCR5380_pread (struct Scsi_Host *instance, 
 *    unsigned char *dst, int len)
 *
 * Purpose : Fast 5380 pseudo-dma read function, transfers len bytes to 
 *    dst
 * 
 * Inputs : dst = destination, len = length in bytes
 *
 * Returns : 0 on success, non zero on a failure such as a watchdog 
 *     timeout.
 */

static inline int NCR5380_pread(struct Scsi_Host *instance, unsigned char *dst, int len) {
    register unsigned char *d = dst;
    /*NCR5380_local_declare();
    NCR5380_setup(instance);*/

    for (; !(NCR5380_read(BUS_AND_STATUS_REG) & BASR_DRQ) ;); /* wait for DRQ */

    for (; len; -- len) *(d ++) = NCR5380_read_dack();

    return 0;
}

/****************************************************************
 * Function : int NCR5380_pwrite (struct Scsi_Host *instance, 
 *    unsigned char *src, int len)
 *
 * Purpose : Fast 5380 pseudo-dma write function, transfers len bytes from
 *    src
 * 
 * Inputs : src = source, len = length in bytes
 *
 * Returns : 0 on success, non zero on a failure such as a watchdog 
 *     timeout.
*/

static inline int NCR5380_pwrite(struct Scsi_Host *instance, unsigned char *src, int len) {
    register unsigned char *s = src;
    /*NCR5380_local_declare();
    NCR5380_setup(instance);*/

    for (; !(NCR5380_read(BUS_AND_STATUS_REG) & BASR_DRQ) ;); /* wait for DRQ */

    for (; len; -- len) NCR5380_write_dack(*(s ++));

    return 0;
}

#include "NCR5380.c"

#ifdef MODULE
/* Eventually this will go into an include file, but this will be later */
Scsi_Host_Template driver_template = MAC_NCR5380;

#include <linux/module.h>
#include "scsi_module.c"
#endif
