#ifndef MAC_SOUND_H
#define MAC_SOUND_H

// ioctl definitions
#define SNDIO_GETBUFSZ 0 /* get the intermediate buffer size */
#define SNDIO_SETLOWRT 1 /* set whether the driver should operate at half the hardware's sample rate */
#define SNDIO_GETLOWRT 2 /* gets whether the driver is operating at half the hardware's sample rate */
#define SNDIO_GETRUNS  3 /* gets how many buffer underruns the driver has encountered since being opened */
#define SNDIO_SETVOL   4 /* sets the sound volume */
#define SNDIO_GETVOL   5 /* gets the sound volume */

// function prototypes
void mac_sound_init(void);
void mac_sound_vblank(void);

#endif
