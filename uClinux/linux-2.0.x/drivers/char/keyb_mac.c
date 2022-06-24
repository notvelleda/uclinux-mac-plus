#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/mm.h>
#include <linux/random.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/keyboard.h>
#include <linux/kd.h>
#include <linux/tqueue.h>
#include <asm/system.h>

#include "../../arch/m68knommu/platform/68000/MacPlus/mac_hw.h"

u_short plain_map[NR_KEYS] = {
	0xfb61,	0xfb73,	0xfb64,	0xfb66,	0xfb68,	0xfb67,	0xfb7a,	0xfb78,
	0xfb63,	0xfb76,	0xf200,	0xfb62,	0xfb71,	0xfb77,	0xfb65,	0xfb72,
	0xfb79,	0xfb74,	0xf031,	0xf032,	0xf033,	0xf034,	0xf036,	0xf035,
	0xf03d,	0xf039,	0xf037,	0xf02d,	0xf038,	0xf030,	0xf05d,	0xfb6f,
	0xfb75,	0xf05b,	0xfb69,	0xfb70,	0xf201,	0xfb6c,	0xfb6a,	0xf027,
	0xfb6b,	0xf03b,	0xf05c,	0xf02c,	0xf02f,	0xfb6e,	0xfb6d,	0xf02e,
	0xf009,	0xf020,	0xf060,	0xf07f,	0xf200,	0xf01b,	0xf702,	0xf702,
	0xf700,	0xf207,	0xf703,	0xf601,	0xf602,	0xf600,	0xf603,	0xf200,
	0xf200,	0xf310,	0xf200,	0xf30c,	0xf200,	0xf30a,	0xf200,	0xf208,
	0xf200,	0xf200,	0xf200,	0xf30d,	0xf30e,	0xf200,	0xf30b,	0xf200,
	0xf200,	0xf200,	0xf300,	0xf301,	0xf302,	0xf303,	0xf304,	0xf305,
	0xf306,	0xf307,	0xf200,	0xf308,	0xf309,	0xf200,	0xf200,	0xf200,
	0xf104,	0xf105,	0xf106,	0xf102,	0xf107,	0xf108,	0xf200,	0xf10a,
	0xf200,	0xf10c,	0xf200,	0xf209,	0xf200,	0xf109,	0xf200,	0xf10b,
	0xf200,	0xf11d,	0xf115,	0xf114,	0xf118,	0xf116,	0xf103,	0xf117,
	0xf101,	0xf119,	0xf100,	0xf700,	0xf703,	0xf702,	0xf200,	0xf200,
};

u_short shift_map[NR_KEYS] = {
	0xfb41,	0xfb53,	0xfb44,	0xfb46,	0xfb48,	0xfb47,	0xfb5a,	0xfb58,
	0xfb43,	0xfb56,	0xf200,	0xfb42,	0xfb51,	0xfb57,	0xfb45,	0xfb52,
	0xfb59,	0xfb54,	0xf021,	0xf040,	0xf023,	0xf024,	0xf05e,	0xf025,
	0xf02b,	0xf028,	0xf026,	0xf05f,	0xf02a,	0xf029,	0xf07d,	0xfb4f,
	0xfb55,	0xf07b,	0xfb49,	0xfb50,	0xf201,	0xfb4c,	0xfb4a,	0xf022,
	0xfb4b,	0xf03a,	0xf07c,	0xf03c,	0xf03f,	0xfb4e,	0xfb4d,	0xf03e,
	0xf009,	0xf020,	0xf07e,	0xf07f,	0xf200,	0xf01b,	0xf702,	0xf702,
	0xf700,	0xf207,	0xf703,	0xf601,	0xf602,	0xf600,	0xf603,	0xf200,
	0xf200,	0xf310,	0xf200,	0xf30c,	0xf200,	0xf30a,	0xf200,	0xf208,
	0xf200,	0xf200,	0xf200,	0xf30d,	0xf30e,	0xf200,	0xf30b,	0xf200,
	0xf200,	0xf200,	0xf300,	0xf301,	0xf302,	0xf303,	0xf304,	0xf305,
	0xf306,	0xf307,	0xf200,	0xf308,	0xf309,	0xf200,	0xf200,	0xf200,
	0xf10e,	0xf10f,	0xf110,	0xf10c,	0xf111,	0xf112,	0xf200,	0xf10a,
	0xf200,	0xf10c,	0xf200,	0xf203,	0xf200,	0xf113,	0xf200,	0xf10b,
	0xf200,	0xf11d,	0xf115,	0xf114,	0xf20b,	0xf116,	0xf10d,	0xf117,
	0xf10b,	0xf20a,	0xf10a,	0xf700,	0xf703,	0xf702,	0xf200,	0xf200,
};

u_short altgr_map[NR_KEYS] = {
	0xf914,	0xfb73,	0xf917,	0xf919,	0xfb68,	0xfb67,	0xfb7a,	0xfb78,
	0xf916,	0xfb76,	0xf200,	0xf915,	0xfb71,	0xfb77,	0xf918,	0xfb72,
	0xfb79,	0xfb74,	0xf200,	0xf040,	0xf200,	0xf024,	0xf200,	0xf200,
	0xf200,	0xf05d,	0xf07b,	0xf05c,	0xf05b,	0xf07d,	0xf07e,	0xfb6f,
	0xfb75,	0xf200,	0xfb69,	0xfb70,	0xf201,	0xfb6c,	0xfb6a,	0xf200,
	0xfb6b,	0xf200,	0xf200,	0xf200,	0xf200,	0xfb6e,	0xfb6d,	0xf200,
	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf702,	0xf702,
	0xf700,	0xf207,	0xf703,	0xf601,	0xf602,	0xf600,	0xf603,	0xf200,
	0xf200,	0xf310,	0xf200,	0xf30c,	0xf200,	0xf30a,	0xf200,	0xf208,
	0xf200,	0xf200,	0xf200,	0xf30d,	0xf30e,	0xf200,	0xf30b,	0xf200,
	0xf200,	0xf200,	0xf90a,	0xf90b,	0xf90c,	0xf90d,	0xf90e,	0xf90f,
	0xf910,	0xf911,	0xf200,	0xf912,	0xf913,	0xf200,	0xf200,	0xf200,
	0xf510,	0xf511,	0xf512,	0xf50e,	0xf513,	0xf514,	0xf200,	0xf516,
	0xf200,	0xf10c,	0xf200,	0xf202,	0xf200,	0xf515,	0xf200,	0xf517,
	0xf200,	0xf11d,	0xf115,	0xf114,	0xf118,	0xf116,	0xf50f,	0xf117,
	0xf50d,	0xf119,	0xf50c,	0xf700,	0xf703,	0xf702,	0xf200,	0xf200,
};

u_short ctrl_map[NR_KEYS] = {
	0xf001,	0xf013,	0xf004,	0xf006,	0xf008,	0xf007,	0xf01a,	0xf018,
	0xf003,	0xf016,	0xf200,	0xf002,	0xf011,	0xf017,	0xf005,	0xf012,
	0xf019,	0xf014,	0xf200,	0xf000,	0xf01b,	0xf01c,	0xf01e,	0xf01d,
	0xf200,	0xf200,	0xf01f,	0xf01f,	0xf07f,	0xf200,	0xf01d,	0xf00f,
	0xf015,	0xf01b,	0xf009,	0xf010,	0xf201,	0xf00c,	0xf00a,	0xf007,
	0xf00b,	0xf200,	0xf01c,	0xf200,	0xf07f,	0xf00e,	0xf00d,	0xf20e,
	0xf200,	0xf000,	0xf000,	0xf008,	0xf200,	0xf200,	0xf702,	0xf702,
	0xf700,	0xf207,	0xf703,	0xf601,	0xf602,	0xf600,	0xf603,	0xf200,
	0xf200,	0xf310,	0xf200,	0xf30c,	0xf200,	0xf30a,	0xf200,	0xf208,
	0xf200,	0xf200,	0xf200,	0xf30d,	0xf30e,	0xf200,	0xf30b,	0xf200,
	0xf200,	0xf200,	0xf300,	0xf301,	0xf302,	0xf303,	0xf304,	0xf305,
	0xf306,	0xf307,	0xf200,	0xf308,	0xf309,	0xf200,	0xf200,	0xf200,
	0xf104,	0xf105,	0xf106,	0xf102,	0xf107,	0xf108,	0xf200,	0xf10a,
	0xf200,	0xf10c,	0xf200,	0xf204,	0xf200,	0xf109,	0xf200,	0xf10b,
	0xf200,	0xf11d,	0xf115,	0xf114,	0xf118,	0xf116,	0xf103,	0xf117,
	0xf101,	0xf119,	0xf100,	0xf700,	0xf703,	0xf702,	0xf200,	0xf200,
};

u_short shift_ctrl_map[NR_KEYS] = {
	0xf001,	0xf013,	0xf004,	0xf006,	0xf008,	0xf007,	0xf01a,	0xf018,
	0xf003,	0xf016,	0xf200,	0xf002,	0xf011,	0xf017,	0xf005,	0xf012,
	0xf019,	0xf014,	0xf200,	0xf000,	0xf200,	0xf200,	0xf200,	0xf200,
	0xf200,	0xf200,	0xf200,	0xf01f,	0xf200,	0xf200,	0xf200,	0xf00f,
	0xf015,	0xf200,	0xf009,	0xf010,	0xf201,	0xf00c,	0xf00a,	0xf200,
	0xf00b,	0xf200,	0xf200,	0xf200,	0xf200,	0xf00e,	0xf00d,	0xf200,
	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf702,	0xf702,
	0xf700,	0xf207,	0xf703,	0xf601,	0xf602,	0xf600,	0xf603,	0xf200,
	0xf200,	0xf310,	0xf200,	0xf30c,	0xf200,	0xf30a,	0xf200,	0xf208,
	0xf200,	0xf200,	0xf200,	0xf30d,	0xf30e,	0xf200,	0xf30b,	0xf200,
	0xf200,	0xf200,	0xf300,	0xf301,	0xf302,	0xf303,	0xf304,	0xf305,
	0xf306,	0xf307,	0xf200,	0xf308,	0xf309,	0xf200,	0xf200,	0xf200,
	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,
	0xf200,	0xf10c,	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,
	0xf200,	0xf11d,	0xf115,	0xf114,	0xf118,	0xf116,	0xf200,	0xf117,
	0xf200,	0xf119,	0xf200,	0xf700,	0xf703,	0xf702,	0xf200,	0xf20c,
};

u_short alt_map[NR_KEYS] = {
	0xf861,	0xf873,	0xf864,	0xf866,	0xf868,	0xf867,	0xf87a,	0xf878,
	0xf863,	0xf876,	0xf200,	0xf862,	0xf871,	0xf877,	0xf865,	0xf872,
	0xf879,	0xf874,	0xf831,	0xf832,	0xf833,	0xf834,	0xf836,	0xf835,
	0xf83d,	0xf839,	0xf837,	0xf82d,	0xf838,	0xf830,	0xf85d,	0xf86f,
	0xf875,	0xf85b,	0xf869,	0xf870,	0xf80d,	0xf86c,	0xf86a,	0xf827,
	0xf86b,	0xf83b,	0xf85c,	0xf82c,	0xf82f,	0xf86e,	0xf86d,	0xf82e,
	0xf809,	0xf820,	0xf860,	0xf87f,	0xf200,	0xf81b,	0xf702,	0xf702,
	0xf700,	0xf207,	0xf703,	0xf210,	0xf211,	0xf600,	0xf603,	0xf200,
	0xf200,	0xf310,	0xf200,	0xf30c,	0xf200,	0xf30a,	0xf200,	0xf208,
	0xf200,	0xf200,	0xf200,	0xf30d,	0xf30e,	0xf200,	0xf30b,	0xf200,
	0xf200,	0xf200,	0xf900,	0xf901,	0xf902,	0xf903,	0xf904,	0xf905,
	0xf906,	0xf907,	0xf200,	0xf908,	0xf909,	0xf200,	0xf200,	0xf200,
	0xf504,	0xf505,	0xf506,	0xf502,	0xf507,	0xf508,	0xf200,	0xf50a,
	0xf200,	0xf10c,	0xf200,	0xf209,	0xf200,	0xf509,	0xf200,	0xf50b,
	0xf200,	0xf11d,	0xf115,	0xf114,	0xf118,	0xf116,	0xf503,	0xf117,
	0xf501,	0xf119,	0xf500,	0xf700,	0xf703,	0xf702,	0xf200,	0xf200,
};

u_short ctrl_alt_map[NR_KEYS] = {
	0xf801,	0xf813,	0xf804,	0xf806,	0xf808,	0xf807,	0xf81a,	0xf818,
	0xf803,	0xf816,	0xf200,	0xf802,	0xf811,	0xf817,	0xf805,	0xf812,
	0xf819,	0xf814,	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,
	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf80f,
	0xf815,	0xf200,	0xf809,	0xf810,	0xf201,	0xf80c,	0xf80a,	0xf200,
	0xf80b,	0xf200,	0xf200,	0xf200,	0xf200,	0xf80e,	0xf80d,	0xf200,
	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf200,	0xf702,	0xf702,
	0xf700,	0xf207,	0xf703,	0xf601,	0xf602,	0xf600,	0xf603,	0xf200,
	0xf200,	0xf310,	0xf200,	0xf30c,	0xf200,	0xf30a,	0xf200,	0xf208,
	0xf200,	0xf200,	0xf200,	0xf30d,	0xf30e,	0xf200,	0xf30b,	0xf200,
	0xf200,	0xf200,	0xf300,	0xf301,	0xf302,	0xf303,	0xf304,	0xf305,
	0xf306,	0xf307,	0xf200,	0xf308,	0xf309,	0xf200,	0xf200,	0xf200,
	0xf504,	0xf505,	0xf506,	0xf502,	0xf507,	0xf508,	0xf200,	0xf50a,
	0xf200,	0xf10c,	0xf200,	0xf200,	0xf200,	0xf509,	0xf200,	0xf50b,
	0xf200,	0xf11d,	0xf115,	0xf114,	0xf118,	0xf116,	0xf503,	0xf117,
	0xf501,	0xf119,	0xf500,	0xf700,	0xf703,	0xf702,	0xf200,	0xf200,
};

ushort *key_maps[MAX_NR_KEYMAPS] = {
    plain_map, shift_map, altgr_map, 0,
    ctrl_map, shift_ctrl_map, 0, 0,
    alt_map, 0, 0, 0,
    ctrl_alt_map, 0
};

unsigned int keymap_count = 7;

/*
 * Philosophy: most people do not define more strings, but they who do
 * often want quite a lot of string space. So, we statically allocate
 * the default and allocate dynamically in chunks of 512 bytes.
 */

char func_buf[] = {
    '\033', '[', '[', 'A', 0, 
    '\033', '[', '[', 'B', 0, 
    '\033', '[', '[', 'C', 0, 
    '\033', '[', '[', 'D', 0, 
    '\033', '[', '[', 'E', 0, 
    '\033', '[', '1', '7', '~', 0, 
    '\033', '[', '1', '8', '~', 0, 
    '\033', '[', '1', '9', '~', 0, 
    '\033', '[', '2', '0', '~', 0, 
    '\033', '[', '2', '1', '~', 0, 
    '\033', '[', '2', '3', '~', 0, 
    '\033', '[', '2', '4', '~', 0, 
    '\033', '[', '2', '5', '~', 0, 
    '\033', '[', '2', '6', '~', 0, 
    '\033', '[', '2', '8', '~', 0, 
    '\033', '[', '2', '9', '~', 0, 
    '\033', '[', '3', '1', '~', 0, 
    '\033', '[', '3', '2', '~', 0, 
    '\033', '[', '3', '3', '~', 0, 
    '\033', '[', '3', '4', '~', 0, 
    '\033', '[', '1', '~', 0, 
    '\033', '[', '2', '~', 0, 
    '\033', '[', '3', '~', 0, 
    '\033', '[', '4', '~', 0, 
    '\033', '[', '5', '~', 0, 
    '\033', '[', '6', '~', 0, 
    '\033', '[', 'M', 0, 
    '\033', '[', 'P', 0, 
};

char *funcbufptr = func_buf;
int funcbufsize = sizeof(func_buf);
int funcbufleft = 0;          /* space left */

char *func_table[MAX_NR_FUNC] = {
    func_buf + 0,
    func_buf + 5,
    func_buf + 10,
    func_buf + 15,
    func_buf + 20,
    func_buf + 25,
    func_buf + 31,
    func_buf + 37,
    func_buf + 43,
    func_buf + 49,
    func_buf + 55,
    func_buf + 61,
    func_buf + 67,
    func_buf + 73,
    func_buf + 79,
    func_buf + 85,
    func_buf + 91,
    func_buf + 97,
    func_buf + 103,
    func_buf + 109,
    func_buf + 115,
    func_buf + 120,
    func_buf + 125,
    func_buf + 130,
    func_buf + 135,
    func_buf + 140,
    func_buf + 145,
    0,
    0,
    func_buf + 149,
    0,
};

struct kbdiacr accent_table[MAX_DIACR] = {
    {'`', 'A', '\300'},    {'`', 'a', '\340'},
    {'\'', 'A', '\301'},    {'\'', 'a', '\341'},
    {'^', 'A', '\302'},    {'^', 'a', '\342'},
    {'~', 'A', '\303'},    {'~', 'a', '\343'},
    {'"', 'A', '\304'},    {'"', 'a', '\344'},
    {'O', 'A', '\305'},    {'o', 'a', '\345'},
    {'0', 'A', '\305'},    {'0', 'a', '\345'},
    {'A', 'A', '\305'},    {'a', 'a', '\345'},
    {'A', 'E', '\306'},    {'a', 'e', '\346'},
    {',', 'C', '\307'},    {',', 'c', '\347'},
    {'`', 'E', '\310'},    {'`', 'e', '\350'},
    {'\'', 'E', '\311'},    {'\'', 'e', '\351'},
    {'^', 'E', '\312'},    {'^', 'e', '\352'},
    {'"', 'E', '\313'},    {'"', 'e', '\353'},
    {'`', 'I', '\314'},    {'`', 'i', '\354'},
    {'\'', 'I', '\315'},    {'\'', 'i', '\355'},
    {'^', 'I', '\316'},    {'^', 'i', '\356'},
    {'"', 'I', '\317'},    {'"', 'i', '\357'},
    {'-', 'D', '\320'},    {'-', 'd', '\360'},
    {'~', 'N', '\321'},    {'~', 'n', '\361'},
    {'`', 'O', '\322'},    {'`', 'o', '\362'},
    {'\'', 'O', '\323'},    {'\'', 'o', '\363'},
    {'^', 'O', '\324'},    {'^', 'o', '\364'},
    {'~', 'O', '\325'},    {'~', 'o', '\365'},
    {'"', 'O', '\326'},    {'"', 'o', '\366'},
    {'/', 'O', '\330'},    {'/', 'o', '\370'},
    {'`', 'U', '\331'},    {'`', 'u', '\371'},
    {'\'', 'U', '\332'},    {'\'', 'u', '\372'},
    {'^', 'U', '\333'},    {'^', 'u', '\373'},
    {'"', 'U', '\334'},    {'"', 'u', '\374'},
    {'\'', 'Y', '\335'},    {'\'', 'y', '\375'},
    {'T', 'H', '\336'},    {'t', 'h', '\376'},
    {'s', 's', '\337'},    {'"', 'y', '\377'},
    {'s', 'z', '\337'},    {'i', 'j', '\377'},
};

unsigned int accent_table_size = 68;

int setkeycode(unsigned int scancode, unsigned int keycode) {
    return -EOPNOTSUPP;
}

int getkeycode(unsigned int scancode) {
    return -EOPNOTSUPP;
}

static struct mac_keyb {
    u_char model;
    u_char next;
    u_char has_next;
    u_char initialized;
    u_char last_command;
    u_char last_response;
    u_char cmd_state;
    u_char next_cmd;
    u_char ticks_since_recv;
    u_char retries;
    u_char send_delay;
} the_keyb = { 0, };

// send command to keyboard
static inline void mac_keyb_send_cmd(struct mac_keyb *keyb, register u_char cmd) {
    //printk("send %02x\n", cmd);
    //for (i = 0; i < 16; i ++);
    cli();
    keyb->last_command = cmd;
    keyb->cmd_state = 0;
    MAC_VIA_SR = 0x0;
    MAC_VIA_ACR = (MAC_VIA_ACR & 0xe3) | 0x18;
    MAC_VIA_SR = 0x0;
    nop();
    MAC_VIA_ACR |= 0x1c;
    MAC_VIA_SR = cmd;
    sti();
}

static inline void queue_send(struct mac_keyb *keyb, register u_char cmd) {
    /*if (keyb->next_cmd) {
        panic("havent sent command!\n");
    } else {*/
        keyb->next_cmd = cmd;
    //}
}

// initialize keyboard
void mac_keyb_init(void) {
    //printk("keyb init!\n");
    MAC_VIA_IER = 0x80 | MAC_VIA_INT_KBRDY | MAC_VIA_INT_KBDAT | MAC_VIA_INT_KBCLK;
    the_keyb.initialized = 0;
    the_keyb.next_cmd = 0;
    the_keyb.ticks_since_recv = 0;
    the_keyb.send_delay = 5;
    queue_send(&the_keyb, MAC_KEYB_MODEL);
}

void mac_keyb_send_queue(void) {
    u_char c;
    if (the_keyb.send_delay) {
        the_keyb.send_delay --;
    } else if (c = the_keyb.next_cmd) {
        the_keyb.next_cmd = 0;
        mac_keyb_send_cmd(&the_keyb, c);
    }
    
    if ((++ the_keyb.ticks_since_recv) > 30) { // 0.5s since last command
        the_keyb.retries ++;
        if (the_keyb.retries > 10) {
            printk("can't contact keyboard!");
            the_keyb.retries = 0;
        }
        mac_keyb_init();
    }
}

extern void process_keycode(int);

static inline void data_int_partial(void) {
    //printk("got keyboard data %02x\n", the_keyb.last_response);
    if (the_keyb.last_command == MAC_KEYB_INQ || the_keyb.last_command == MAC_KEYB_INST) { // got key transition or null
        if (the_keyb.last_response != MAC_KEYB_NULL) {
            u_char code, state;
            // get key transition info
            the_keyb.last_response >>= 1;
            code = the_keyb.last_response & 0x3f;
            the_keyb.last_response >>= 6;
            state = the_keyb.last_response;

            //printk("got key %02x (%s)!\n", code, state ? "up" : "down");

            if (code == 0x39) {
                // if the key is CAPS, fake a press/release.
                process_keycode(0x39);
                process_keycode(0x80 | 0x39);
            } else {
                if (state) {
                    code |= 0x80;
                }

                process_keycode(code);
            }
        } else {
            //printk("no key transitions\n");
        }
        //printk("");
        queue_send(&the_keyb, MAC_KEYB_INQ); // poll for key press
    } else if (the_keyb.last_command == MAC_KEYB_MODEL) { // got model number
        if (the_keyb.last_response & 0x1) {
            // get keyboard information
            the_keyb.last_response >>= 1;
            the_keyb.model = (the_keyb.last_response) & 0x7;
            the_keyb.last_response >>= 3;
            the_keyb.next = (the_keyb.last_response) & 0x7;
            the_keyb.last_response >>= 3;
            the_keyb.has_next = (the_keyb.last_response) & 0x1;
            the_keyb.initialized = 1;
            //printk("keyboard: model %02x, next %02x, has next %s\n", the_keyb.model, the_keyb.next, the_keyb.has_next ? "1" : "0");
            //printk("");
            the_keyb.send_delay = 5;
            queue_send(&the_keyb, MAC_KEYB_INQ); // poll for key press
        } else {
            printk("got %02x (cmd %02x), reinit\n", the_keyb.last_response, the_keyb.last_command);
            mac_keyb_init();
        }
    } else if (the_keyb.last_command == MAC_KEYB_TEST) { // got test result
        if (the_keyb.last_response == MAC_KEYB_ACK) {
            printk("keyboard test ok!\n");
        } else if (the_keyb.last_response == MAC_KEYB_NACK) {
            printk("keyboard test not ok!\n");
        } else {
            printk("got %02x (cmd %02x), reinit\n", the_keyb.last_response, the_keyb.last_command);
            mac_keyb_init();
        }
    } else {
        printk("got unknown data %02x\n", the_keyb.last_response);
    }
}

void mac_keyb_int_ready(void) {
    u_char i;

    cli();

    if (the_keyb.cmd_state == 0) {
        //for (i = 0; i < 64; i ++);
        __asm__ __volatile__(
            "move #0xc8, %%d0\n\t"
            "lsl #0x3, %%d0\n\t"
            "loop: dbf %%d0, loop\n\t"
            :
            :
            : "d0"
        );

        MAC_VIA_ACR = (MAC_VIA_ACR & 0xe3) | 0xc;

        MAC_VIA_SR = 0;

        the_keyb.cmd_state = 1;

        sti();
        //printk("cmd state now 1\n");
    } else if (the_keyb.cmd_state == 1) {
        for (i = 0; i < 8; i ++);

        the_keyb.last_response = MAC_VIA_SR;
        the_keyb.ticks_since_recv = 0;
        the_keyb.cmd_state = 2;
        the_keyb.retries = 0;

        sti();

        //printk("got %02x\n", the_keyb.last_response);

        data_int_partial();
    }
}
