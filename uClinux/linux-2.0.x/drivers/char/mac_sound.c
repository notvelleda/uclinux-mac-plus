// very basic driver for Mac Plus audio, copyright 2022 velleda
// accepts 22255.5 Hz or 11127.75 Hz (yes really) 8-bit unsigned PCM audio
// write() calls will return zero if buffers are full, so more data can be loaded
// has ioctls for configuration, see mac_sound.h for details

// to use: create a character device with mknod with the sound major number (typically 14, device minor number doesn't matter)
// and write() uncompressed audio data into it. you could cat into it, however it generally can't keep up

#include <linux/fs.h>
#include <linux/major.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <asm/system.h>
#include "../../arch/m68knommu/platform/68000/MacPlus/mac_hw.h"
#include "mac_sound.h"

static void mac_sound_bh(void);
static int mac_sound_write(struct inode *p_inode, struct file *p_file, const char *buffer, int length);
static int mac_sound_ioctl(struct inode *p_inode, struct file *p_file, unsigned int cmd, unsigned long arg);
static int mac_sound_open(struct inode *p_inode, struct file *p_file);
static void mac_sound_release(struct inode *p_inode, struct file *p_file);

struct file_operations mac_sound_fops = {
    NULL,                /* lseek   */
    NULL,                /* read    */
    mac_sound_write,     /* write   */
    NULL,                /* readdir */
    NULL,                /* select  */
    mac_sound_ioctl,     /* ioctl   */
    NULL,                /* mmap    */
    mac_sound_open,      /* open    */
    mac_sound_release    /* release */
};

static int sound_enabled = 0; // whether sound is enabled
static int underruns = 0; // how many buffer underruns we've encountered

#define BUF_LEN 370

static u8 *const buffers[2] = { 0x3ffd00, 0x3fa100 }; // addresses of hardware sound buffers
static u8 cur_buf = 0; // current hardware buffer

static u8 * raw_buf[2] = { NULL, NULL }; // intermediate sound buffers
static int raw_buf_pos = 0; // playback position in intermediate buffer
static int raw_buf_size = BUF_LEN * 5; // size of intermediate buffers, has to be a multiple of BUF_LEN
static int cur_raw_buf = 0; // which intermediate buffer can we fill?
static int cur_buf_fill_amt = 0; // how much of our current intermediate buffer have we filled?

static int have_filled_buffer = 0; // has the hardware sound buffer been filled?
static int need_filled_buffer = 1; // do we need our current intermediate buffer filled?

static int low_sample_rate = 0; // should we play at half the actual sample rate? this is useful to save file size

static int first_sample = 0; // is this the first sample of the stream? used to only start outputting audio when valid samples are available

// initializes the sound driver
void mac_sound_init(void) {
    int i;

    for (i = 0; i < BUF_LEN; i ++) { // fill buffers with silence, not that it really matters
        buffers[0][i * 2] = buffers[1][i * 2] = 0xff;
    }

    //MAC_VIA_BUFA |= 1 << 3; // set volume

    // register our device major number. there isn't gonna be any other sound system so we can just take all the minor device ids
    if (register_chrdev(SOUND_MAJOR, "sound", &mac_sound_fops)) {
        printk("mac_sound: unable to get major %d for sound devs\n", SOUND_MAJOR);
        sound_enabled = -1;
        return;
    }

    // register the bottom half interrupt handler
    init_bh(SOUND_BH, mac_sound_bh);

    // allocate intermediate buffers
    if (!(raw_buf[0] = kmalloc(raw_buf_size, GFP_KERNEL))) {
        printk("mac_sound: unable to allocate buffers for sound\n");
        sound_enabled = -1;
        return;
    }

    if (!(raw_buf[1] = kmalloc(raw_buf_size, GFP_KERNEL))) {
        kfree(raw_buf[0]);
        printk("mac_sound: unable to allocate buffers for sound\n");
        sound_enabled = -1;
        return;
    }

    printk("Mac Plus sound driver initialized (2x size %d buffers)\n", raw_buf_size);
}

// run on every vblank interrupt, switches sound buffers in the VIA and frees up the previous one
void mac_sound_vblank(void) {
    // if we're playing sound, just switch audio buffers and mark our bottom half handler
    // we don't want to do actual processing here since that's slow and will bog down the system
    if (have_filled_buffer) {
        register u8 temp = MAC_VIA_BUFA ^ (1 << 3); // hopefully storing this temp value in a register or something speeds this up
        MAC_VIA_BUFA = temp;
        cur_buf = ((temp & (1 << 3)) > 0) ? 1 : 0;
        have_filled_buffer = 0;

        mark_bh(SOUND_BH);
    }
}

// bottom half of the interrupt routine, fills the next sound buffer from our buffer
static void mac_sound_bh(void) {
    register u8 temp_cur_buf = !cur_buf;
    register u8 temp_cur_raw = !cur_raw_buf;

    if (have_filled_buffer) {
        printk("mac_sound: can't fill next buffer!\n");
        return;
    }

    // fill next hardware buffer from full intermediate buffer
    if (low_sample_rate) {
        // the audio buffer switch is horrendously delayed for some reason, so instead of switching buffers around vblank or the start of the frame,
        // they instead switch 3 samples into the new frame. therefore, in every buffer, the first 3 samples will be from the previous buffer
        // we have to go out of our way to correct for this. it's a lot more annoying in low sample rate mode since we duplicate every other sample
        // TODO: check behavior on real hardware, hopefully this is consistent
        register int i = 2;

        // duplicate the first sample into the first 2 samples of the buffer
        register u8 sample = ~raw_buf[temp_cur_raw][raw_buf_pos];
        buffers[temp_cur_buf][0] = sample;
        buffers[temp_cur_buf][2] = sample;

        // duplicate the second sample into the 3rd sample of the buffer
        sample = ~raw_buf[temp_cur_raw][raw_buf_pos + 1];
        buffers[temp_cur_buf][4] = sample;

        // switch to the proper buffer
        temp_cur_buf = !temp_cur_buf;

        // populate the 4th sample of the buffer
        buffers[temp_cur_buf][6] = sample;

        // populate the rest of the samples
        for (; i < BUF_LEN / 2; i ++) {
            register u8 sample = ~raw_buf[temp_cur_raw][raw_buf_pos + i];
            register int index = i << 2;
            buffers[temp_cur_buf][index] = sample;
            index += 2;
            buffers[temp_cur_buf][index] = sample;
        }

        raw_buf_pos += BUF_LEN / 2;
    } else {
        register int i = 3;

        /*for (; i < 3; i ++) {
            buffers[temp_cur_buf][i << 1] = ~raw_buf[temp_cur_raw][raw_buf_pos + i];
        }*/

        // populate the first 3 samples of the buffer
        buffers[temp_cur_buf][0] = ~raw_buf[temp_cur_raw][raw_buf_pos]; // manual loop unrolling (:
        buffers[temp_cur_buf][2] = ~raw_buf[temp_cur_raw][raw_buf_pos + 1];
        buffers[temp_cur_buf][4] = ~raw_buf[temp_cur_raw][raw_buf_pos + 2];

        // switch buffers
        temp_cur_buf = !temp_cur_buf;

        // populate the rest of the buffer
        for (; i < BUF_LEN; i ++) {
            buffers[temp_cur_buf][i << 1] = ~raw_buf[temp_cur_raw][raw_buf_pos + i];
        }

        raw_buf_pos += BUF_LEN;
    }

    have_filled_buffer = 1;

    // switch out intermediate buffers if we've played all the samples in the current one
    if (raw_buf_pos >= raw_buf_size) {
        int old_fill_amt;

        raw_buf_pos = 0;
        cur_raw_buf = !cur_raw_buf;

        old_fill_amt = cur_buf_fill_amt;
        cur_buf_fill_amt = 0;

        need_filled_buffer = 1;

        if (old_fill_amt < raw_buf_size) {
            //printk("buffer underrun\n");
            underruns ++;
        }
    }
}

/*
static int mac_sound_read(struct inode *p_inode, struct file *p_file, char *buffer, int length) {
    return -EINVAL;
}
*/

static int mac_sound_write(struct inode *p_inode, struct file *p_file, const char *buffer, int length) {
    if (first_sample && cur_buf_fill_amt >= raw_buf_size) {
        first_sample = 0;
        mark_bh(SOUND_BH);
    } else if (!need_filled_buffer || cur_buf_fill_amt >= raw_buf_size)
        return 0; // <- full of audio samples

    if (cur_buf_fill_amt + length >= raw_buf_size) { // have we been given too much data?
        int actual_length = raw_buf_size - cur_buf_fill_amt;

        memcpy(raw_buf[cur_raw_buf] + cur_buf_fill_amt, buffer, actual_length);

        cur_buf_fill_amt = raw_buf_size;

        if (first_sample) { // kick off bottom half handler if we're just starting playing
            first_sample = 0;
            mark_bh(SOUND_BH);
        }

        return actual_length;
    } else {
        memcpy(raw_buf[cur_raw_buf] + cur_buf_fill_amt, buffer, length);

        cur_buf_fill_amt += length;

        return length;
    }
}

/*
static int mac_sound_select(struct inode *, struct file *, int, select_table *) {
    return -EINVAL;
}
*/

static int mac_sound_ioctl(struct inode *p_inode, struct file *p_file, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case SNDIO_GETBUFSZ:
            return raw_buf_size;
        case SNDIO_SETLOWRT:
            low_sample_rate = (arg > 0) ? 1 : 0;
            return 0;
        case SNDIO_GETLOWRT:
            return low_sample_rate;
        case SNDIO_GETRUNS:
            return underruns;
        case SNDIO_SETVOL:
            if (arg > 7) return -EINVAL;
            MAC_VIA_BUFA = (MAC_VIA_BUFA & ~7) | (u8) arg;
            return 0;
        case SNDIO_GETVOL:
            return MAC_VIA_BUFA & 7;
        default:
            return -EINVAL;
    }
}

static int mac_sound_open(struct inode *p_inode, struct file *p_file) {
    if (sound_enabled < 0) return -EINVAL;

    if (!sound_enabled) {
        // clear intermediate buffers
        memset(raw_buf[0], 0, raw_buf_size);
        memset(raw_buf[1], 0, raw_buf_size);

        MAC_VIA_BUFB &= ~(1 << 7); // enable sound

        underruns = 0;
        sound_enabled = 1;

        need_filled_buffer = 1;
        have_filled_buffer = 0;
        cur_buf_fill_amt = 0;
        first_sample = 1;
    }

    return 0;
}

static void mac_sound_release(struct inode *p_inode, struct file *p_file) {
    sound_enabled = 0;
    first_sample = 0;
    have_filled_buffer = 0;
    MAC_VIA_BUFB |= 1 << 7; // disable sound
    //printk("%d buffer underruns\n", underruns);
}
