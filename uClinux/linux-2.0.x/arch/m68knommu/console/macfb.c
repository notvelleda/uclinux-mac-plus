#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/tty.h>
#include <linux/malloc.h>
#include <linux/delay.h>
#include <linux/config.h>
#include <asm/segment.h>
#include <asm/system.h>
#include <asm/irq.h>
#include <asm/pgtable.h>
#include <linux/fb.h>

#define arraysize(x) (sizeof(x) / sizeof(*(x)))

#define SCRN_WIDTH 512
#define SCRN_HEIGHT 342

#define SCRN_BASE 0x3fa700
#define SCRN_BASE_VAR *((unsigned long *) 0x0824)
#define SCRN_LEN ((SCRN_WIDTH / 8) * SCRN_HEIGHT)

static int mac_fb_get_fix(struct fb_fix_screeninfo *fix, int con);
int mac_fb_get_var(struct fb_var_screeninfo *var, int con);
static int mac_fb_set_var(struct fb_var_screeninfo *var, int con);
static int mac_fb_get_cmap(struct fb_cmap *cmap, int kspc, int con);
static int mac_fb_set_cmap(struct fb_cmap *cmap, int kspc, int con);
static int mac_fb_pan_display(struct fb_var_screeninfo *var, int con);
static int mac_fb_ioctl(struct inode *inode, struct file *file, u_int cmd, u_long arg, int con);

struct fb_info *mac_fb_init();
static int mac_fb_switch(int con);
static int mac_fb_updatevar(int con);
static void mac_fb_blank(int blank);
static int get_video_mode(const char *name);

static struct display disp[MAX_NR_CONSOLES];
static struct fb_info fb_info;

static int node;

static char mac_fb_id[16] = "mac";

static char mac_fb_name[16] = "Mac Builtin";

static struct fb_var_screeninfo mac_fb_predefined[] = {
    {
        SCRN_WIDTH, SCRN_HEIGHT, SCRN_WIDTH, SCRN_HEIGHT,
        0, 0,
        1, -1,
        {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0},
        0, 0,
        -1, -1, /* phys height, width */
        FB_ACCEL_NONE,
        0, 0, 0, 0, 0, 0, 0, /* timing */
        0, /* sync */
        FB_VMODE_NONINTERLACED
    }
};

#define NUM_TOTAL_MODES 1

static int mac_fb_get_fix(struct fb_fix_screeninfo *fix, int con) {
    int i;

    strcpy(fix->id, mac_fb_id);
    fix->smem_start = SCRN_BASE_VAR; // get screen ram address
    fix->smem_len = SCRN_LEN;

    fix->type = FB_TYPE_PACKED_PIXELS;
    fix->type_aux = 0;
    fix->visual = FB_VISUAL_MONO01;

    fix->xpanstep = 0;
    fix->ypanstep = 0;
    fix->ywrapstep = 0;

    fix->line_length = SCRN_WIDTH / 8;

    for (i = 0; i < arraysize(fix->reserved); i++)
        fix->reserved[i] = 0;

    return 0;
}

int mac_fb_get_var(struct fb_var_screeninfo *var, int con) {
    int i;

    var->xres = SCRN_WIDTH;
    var->yres = SCRN_HEIGHT;
    var->xres_virtual = SCRN_WIDTH;
    var->yres_virtual = SCRN_HEIGHT;
    var->xoffset = 0;
    var->yoffset = 0;

    var->bits_per_pixel = 1;
    var->grayscale = -1;

    var->nonstd = 0;
    var->activate = 0;

    var->width = 180; // approximation
    var->height = 135;

    var->accel = FB_ACCEL_NONE;
    var->vmode = FB_VMODE_NONINTERLACED;

    /* Dummy values */

    var->pixclock = 0;
    var->sync = 0;
    var->left_margin = 0;
    var->right_margin = 0;
    var->upper_margin = 0;
    var->lower_margin = 0;
    var->hsync_len = 0;
    var->vsync_len = 0;

    for (i = 0; i < arraysize(var->reserved); i++)
        var->reserved[i] = 0;

    return 0;
}

static int mac_fb_set_var(struct fb_var_screeninfo *var, int con) {
   return -EINVAL;
}

static int mac_fb_get_cmap(struct fb_cmap *cmap, int kspc, int con) {
   return -EINVAL;
}

static int mac_fb_set_cmap(struct fb_cmap *cmap, int kspc, int con) {
   return -EINVAL;
}

static int mac_fb_pan_display(struct fb_var_screeninfo *var, int con) {
   return -EINVAL;
}

static int mac_fb_ioctl(struct inode *inode, struct file *file, u_int cmd, u_long arg, int con) {
   return -EINVAL;
}

static struct fb_ops mac_fb_ops = {
   mac_fb_get_fix, mac_fb_get_var, mac_fb_set_var, mac_fb_get_cmap,
   mac_fb_set_cmap, mac_fb_pan_display, mac_fb_ioctl
};

struct fb_info *mac_fb_init() {
    int err;
    struct fb_fix_screeninfo fix;
    struct fb_var_screeninfo var;

    err = register_framebuffer(mac_fb_name, &node, &mac_fb_ops,
                                NUM_TOTAL_MODES, mac_fb_predefined);
    
    if (err < 0)
        panic("Cannot register frame buffer\n");

    strcpy(fb_info.modename, mac_fb_name);
    fb_info.disp = disp;
    fb_info.switch_con = &mac_fb_switch;
    fb_info.updatevar = &mac_fb_updatevar;
    fb_info.blank = &mac_fb_blank;

    mac_fb_get_fix(&fix, 0);

    disp[0].screen_base = (u_char *) fix.smem_start;
    disp[0].visual = fix.visual;
    disp[0].type = fix.type;
    disp[0].type_aux = fix.type_aux;
    disp[0].ypanstep = fix.ypanstep;
    disp[0].ywrapstep = fix.ywrapstep;
    disp[0].can_soft_blank = 0;
    disp[0].inverse = 1; // mac framebuffer uses 1 for black
    
    mac_fb_get_var(&disp[0].var, 0);

    memset((void *) SCRN_BASE, 0xff, SCRN_LEN); // clear screen

    return &fb_info;
}

static int mac_fb_switch(int con) {
    return -EINVAL;
}

static int mac_fb_updatevar(int con) {
    return -EINVAL;
}

static void mac_fb_blank(int blank) {
    // we don't have this
}
