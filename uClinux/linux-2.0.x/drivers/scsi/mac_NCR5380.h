/*
 * Mac Plus SCSI driver, based on basically every other NCR5380 driver here
 * (c) 2022 velleda
 */

#ifndef MAC_NCR5380_H
#define MAC_NCR5380_H

#include "NCR5380.h"

int mac_scsi_detect(Scsi_Host_Template * tpnt);
int mac_scsi_biosparam(Disk * disk, kdev_t dev, int * ip);

int mac_scsi_queue_command(Scsi_Cmnd *cmd, void (*done)(Scsi_Cmnd *));
int mac_scsi_abort(Scsi_Cmnd *cmd);
int mac_scsi_reset(Scsi_Cmnd *cmd, unsigned int dummy);
int mac_scsi_proc_info(char *buffer, char **start, off_t offset, int length, int hostno, int inout);

#ifndef NULL
#define NULL 0
#endif

#ifndef CMD_PER_LUN
#define CMD_PER_LUN 2
#endif

#ifndef CAN_QUEUE
#define CAN_QUEUE 16
#endif

#define MAC_NCR5380 {NULL, NULL, NULL, \
	mac_scsi_proc_info, \
	"Mac Plus SCSI", 		\
	mac_scsi_detect, NULL, NULL,					\
	NULL, mac_scsi_queue_command, mac_scsi_abort, mac_scsi_reset, NULL, 	\
	mac_scsi_biosparam, 						\
	/* can queue */ CAN_QUEUE, /* id */ 7, SG_ALL,			\
	/* cmd per lun */ CMD_PER_LUN , 0, 0, DISABLE_CLUSTERING}

#define MAC_SCSI_BASE       0x580000

#define MAC_SCSI_READ       (MAC_SCSI_BASE | 0x0)
#define MAC_SCSI_WRITE      (MAC_SCSI_BASE | 0x1)

#define NCR5380_write(r, v) *((volatile u_char *) (MAC_SCSI_WRITE | (r << 4))) = (u_char) (v)
#define NCR5380_read(r) *((volatile u_char *) (MAC_SCSI_READ | (r << 4)))

/* for pseudo DMA in mac_NCR5380.c */
#define NCR5380_write_dack(v) *((volatile u_char *) (MAC_SCSI_WRITE | (OUTPUT_DATA_REG << 4) | 0x200)) = (u_char) (v)
#define NCR5380_read_dack() *((volatile u_char *) (MAC_SCSI_READ | (INPUT_DATA_REG << 4) | 0x200))

#define NCR5380_queue_command mac_scsi_queue_command
#define NCR5380_abort mac_scsi_abort
#define NCR5380_reset mac_scsi_reset
#define NCR5380_proc_info mac_scsi_proc_info

#endif /* MAC_NCR5380_H */
