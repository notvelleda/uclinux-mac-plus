const char font[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // (space)
    0x00, 0x00, 0x5F, 0x00, 0x00, // !
    0x00, 0x07, 0x00, 0x07, 0x00, // "
    0x14, 0x7F, 0x14, 0x7F, 0x14, // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
    0x23, 0x13, 0x08, 0x64, 0x62, // %
    0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x1C, 0x22, 0x41, 0x00, // (
    0x00, 0x41, 0x22, 0x1C, 0x00, // )
    0x08, 0x2A, 0x1C, 0x2A, 0x08, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x50, 0x30, 0x00, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x00, 0x08, 0x14, 0x22, 0x41, // <
    0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x41, 0x22, 0x14, 0x08, 0x00, // >
    0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x32, 0x49, 0x79, 0x41, 0x3E, // @
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x01, 0x01, // F
    0x3E, 0x41, 0x41, 0x51, 0x32, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x04, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x7F, 0x20, 0x18, 0x20, 0x7F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x03, 0x04, 0x78, 0x04, 0x03, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x00, 0x7F, 0x41, 0x41, // [
    0x02, 0x04, 0x08, 0x10, 0x20, // "\"
    0x41, 0x41, 0x7F, 0x00, 0x00, // ]
    0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x01, 0x02, 0x04, 0x00, // `
    0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x08, 0x14, 0x54, 0x54, 0x3C, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x20, 0x40, 0x44, 0x3D, 0x00, // j
    0x00, 0x7F, 0x10, 0x28, 0x44, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x7C, 0x14, 0x14, 0x14, 0x08, // p
    0x08, 0x14, 0x14, 0x18, 0x7C, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x0C, 0x50, 0x50, 0x50, 0x3C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x08, 0x36, 0x41, 0x00, // {
    0x00, 0x00, 0x7F, 0x00, 0x00, // |
    0x00, 0x41, 0x36, 0x08, 0x00, // }
    0x08, 0x08, 0x2A, 0x1C, 0x08, // ->
    0x08, 0x1C, 0x2A, 0x08, 0x08, // <-
    0x00, 0x06, 0x09, 0x09, 0x06  // degree symbol
};

#define SCRN_BASE 0x3fa700
#define SCRN_LEN 0x5580
#define SCRN_LEN_CONSOLE 0x5400

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 342
#define CONSOLE_HEIGHT 336 // console height has to be a multiple of 8!

void drawString(const char *str, short x, short y);
void drawChar(char c, short x, short y);
void drawNum(unsigned int num, short x, short y);
void drawStringFast(const char *str, short x, short y);
void drawNumFast(unsigned int num, short x, short y);

void print(const char *str);
void println(const char *str);
void printNum(unsigned int num);
void clear();

void *memcpy(void *to, const void *from, unsigned long n);
void *memset(void *s, int c, unsigned long count);
void *memmove(void *dest, const void *src, unsigned long n);
void *memmove_progress(void *dest, const void *src, unsigned long n);

void isr();

void hideCursor();

int consoleX = 0;
int consoleY = 0;

char numBuf[9];

// where to put the kernel
#define KERNEL_ADDR 0x1000

struct BootInfo {
    char *arguments;
    char *kernelPtr;
    unsigned long kernelSize;
    char *initrdPtr;
    unsigned long initrdSize;
    int hasInitrd;
};

void start() {
    struct BootInfo *info = (struct BootInfo *) 0x380000;

    asm("jsr 0x00401ca8"); // hide cursor
    asm("movew #0x2700, %sr"); // hehe no more interrupts

    clear();
    
    println("stage 2\n");

    memmove((void *) KERNEL_ADDR, info->kernelPtr, info->kernelSize);

    ((void(*)(void)) KERNEL_ADDR)();

    for (;;); // loop forever- we can't exactly return
}

#define SET_PIXEL(x, y) *((char *) SCRN_BASE + ((x) >> 3) + ((y) << 6)) &= ~(1 << (7 - ((x) & 0x7)))
#define CLEAR_PIXEL(x, y) *((char *) SCRN_BASE + ((x) >> 3) + ((y) << 6)) |= 1 << (7 - ((x) & 0x7))

void drawString(const char *str, short x, short y) {
    short ptr = 0;
    char c;

    while ((c = str[ptr ++]) != 0) {
        drawChar(c, x, y);
        x += 6;
        if (x >= SCREEN_WIDTH - 6) {
            x = 0;
            y += 8;
        }
    }
}

void drawChar(char c, short x, short y) { // slow but functional!
    short index = (c - 0x20) * 5;
    char col, row;
    for (col = 0; col < 5; col ++) {
        short workingRow = y;
        for (row = 0; row < 7; row ++) {
            char pixel = (font[index] >> row) & 0x1;
            if (pixel) {
                SET_PIXEL(x, workingRow);
            } else {
                CLEAR_PIXEL(x, workingRow);
            }
            workingRow ++;
        }
        x ++;
        index ++;
    }
}

void drawStringFast(const char *str, short x, short y) {
    short ptr = 0;
    int idxByte = SCRN_BASE + (x >> 3) + (y << 6);
    short idxSub = 7 - (x & 0x7);
    char c, col, row;
    short fontIndex;

    while ((c = str[ptr ++]) != 0) {
        fontIndex = (c - 0x20) * 5;
        for (col = 0; col < 5; col ++) {
            for (row = 0; row < 7; row ++) {
                char pixel = (font[fontIndex] >> row) & 0x1;
                if (pixel) {
                    *((char *) idxByte + (row << 6)) &= ~(1 << idxSub);
                }
                // don't bother clearing pixels, prolly doesnt matter
            }

            if (-- idxSub < 0) {
                idxSub = 7;
                idxByte ++;
            }
            fontIndex ++;
        }

        if (-- idxSub < 0) {
            idxSub = 7;
            idxByte ++;
        }

        x += 6;
        if (x >= SCREEN_WIDTH - 6) {
            x = 0;
            y += 8;

            idxByte = SCRN_BASE + (x >> 3) + (y << 6);
            idxSub = 7 - (x & 0x7);
        }
    }
}

void print(const char *str) {
    short ptr = 0;
    int idxByte = SCRN_BASE + (consoleX >> 3) + (consoleY << 6);
    short idxSub = 7 - (consoleX & 0x7);
    char c, col, row;
    short fontIndex;

    while ((c = str[ptr ++]) != 0) {
        fontIndex = (c - 0x20) * 5;
        if (c == '\n') {
            consoleX = 0;
            consoleY += 8;

            if (consoleY >= CONSOLE_HEIGHT) {
                memmove((void *) SCRN_BASE, (void *) SCRN_BASE + 512, SCRN_LEN_CONSOLE - 512);
                memset((void *) SCRN_BASE + SCRN_LEN_CONSOLE - 512, 0xff, 512);
                consoleY = CONSOLE_HEIGHT - 8;
            }

            idxByte = SCRN_BASE + (consoleX >> 3) + (consoleY << 6);
            idxSub = 7 - (consoleX & 0x7);
        } else {
            if (c == ' ') {
                consoleX += 6;
                idxByte = SCRN_BASE + (consoleX >> 3) + (consoleY << 6);
                idxSub = 7 - (consoleX & 0x7);
                consoleX -= 6;
            } else {
                for (col = 0; col < 5; col ++) {
                    for (row = 0; row < 7; row ++) {
                        char pixel = (font[fontIndex] >> row) & 0x1;
                        if (pixel) {
                            *((char *) idxByte + (row << 6)) &= ~(1 << idxSub);
                        }
                        // don't bother clearing pixels, prolly doesnt matter
                    }

                    if (-- idxSub < 0) {
                        idxSub = 7;
                        idxByte ++;
                    }
                    fontIndex ++;
                }

                if (-- idxSub < 0) {
                    idxSub = 7;
                    idxByte ++;
                }
            }

            consoleX += 6;
            if (consoleX >= SCREEN_WIDTH - 6) {
                consoleX = 0;
                consoleY += 8;

                if (consoleY >= CONSOLE_HEIGHT) {
                    memmove((void *) SCRN_BASE, (void *) SCRN_BASE + 512, SCRN_LEN_CONSOLE - 512);
                    memset((void *) SCRN_BASE + SCRN_LEN_CONSOLE - 512, 0xff, 512);
                    consoleY = CONSOLE_HEIGHT - 8;
                }

                idxByte = SCRN_BASE + (consoleX >> 3) + (consoleY << 6);
                idxSub = 7 - (consoleX & 0x7);
            }
        }
    }
}

void printNum(unsigned int num) {
    char i;
    for (i = 7; i >= 0; i --) {
        numBuf[i] = "0123456789abcdef"[num & 0xf];
        num >>= 4;
    }
    numBuf[8] = 0;
    print(numBuf);
}

void drawNum(unsigned int num, short x, short y) {
    char i;
    for (i = 7; i >= 0; i --) {
        numBuf[i] = "0123456789abcdef"[num & 0xf];
        num >>= 4;
    }
    numBuf[8] = 0;
    drawString(numBuf, x, y);
}

void drawNumFast(unsigned int num, short x, short y) {
    char i;
    for (i = 7; i >= 0; i --) {
        numBuf[i] = "0123456789abcdef"[num & 0xf];
        num >>= 4;
    }
    numBuf[8] = 0;
    drawStringFast(numBuf, x, y);
}

void println(const char *str) {
    print(str);
    consoleX = 0;
    consoleY += 8;

    if (consoleY >= CONSOLE_HEIGHT) {
        memmove((void *) SCRN_BASE, (void *) SCRN_BASE + 512, SCRN_LEN_CONSOLE - 512);
        memset((void *) SCRN_BASE + SCRN_LEN_CONSOLE - 512, 0xff, 512);
        consoleY = CONSOLE_HEIGHT - 8;
    }
}

void clear() {
    memset((void *) SCRN_BASE, 0xff, SCRN_LEN);
    consoleX = 0;
    consoleY = 0;
}

// not mine lmao
void *memcpy(void *to, const void *from, unsigned long n) {
    void *xto = to;
    unsigned long temp;

    if (!n)
        return xto;
    if ((long)to & 1) {
        char *cto = to;
        const char *cfrom = from;
        *cto++ = *cfrom++;
        to = cto;
        from = cfrom;
        n--;
    }
    if ((long)from & 1) {
        char *cto = to;
        const char *cfrom = from;
        for (; n; n--)
            *cto++ = *cfrom++;
        return xto;
    }
    if (n > 2 && (long)to & 2) {
        short *sto = to;
        const short *sfrom = from;
        *sto++ = *sfrom++;
        to = sto;
        from = sfrom;
        n -= 2;
    }
    temp = n >> 2;
    if (temp) {
        long *lto = to;
        const long *lfrom = from;
        for (; temp; temp--)
            *lto++ = *lfrom++;
        to = lto;
        from = lfrom;
    }
    if (n & 2) {
        short *sto = to;
        const short *sfrom = from;
        *sto++ = *sfrom++;
        to = sto;
        from = sfrom;
    }
    if (n & 1) {
        char *cto = to;
        const char *cfrom = from;
        *cto = *cfrom;
    }
    return xto;
}

void *memset(void *s, int c, unsigned long count) {
    void *xs = s;
    unsigned long temp;

    if (!count)
        return xs;
    c &= 0xff;
    c |= c << 8;
    c |= c << 16;
    if ((long)s & 1) {
        char *cs = s;
        *cs++ = c;
        s = cs;
        count--;
    }
    if (count > 2 && (long)s & 2) {
        short *ss = s;
        *ss++ = c;
        s = ss;
        count -= 2;
    }
    temp = count >> 2;
    if (temp) {
        long *ls = s;
        for (; temp; temp--)
            *ls++ = c;
        s = ls;
    }
    if (count & 2) {
        short *ss = s;
        *ss++ = c;
        s = ss;
    }
    if (count & 1) {
        char *cs = s;
        *cs = c;
    }
    return xs;
}

void *memmove(void *dest, const void *src, unsigned long n) {
    void *xdest = dest;
    unsigned long temp;

    if (!n)
        return xdest;

    if (dest < src) {
        if ((long)dest & 1) {
            char *cdest = dest;
            const char *csrc = src;
            *cdest++ = *csrc++;
            dest = cdest;
            src = csrc;
            n--;
        }
        if (n > 2 && (long)dest & 2) {
            short *sdest = dest;
            const short *ssrc = src;
            *sdest++ = *ssrc++;
            dest = sdest;
            src = ssrc;
            n -= 2;
        }
        temp = n >> 2;
        if (temp) {
            long *ldest = dest;
            const long *lsrc = src;
            temp--;
            do
                *ldest++ = *lsrc++;
            while (temp--);
            dest = ldest;
            src = lsrc;
        }
        if (n & 2) {
            short *sdest = dest;
            const short *ssrc = src;
            *sdest++ = *ssrc++;
            dest = sdest;
            src = ssrc;
        }
        if (n & 1) {
            char *cdest = dest;
            const char *csrc = src;
            *cdest = *csrc;
        }
    } else {
        dest = (char *)dest + n;
        src = (const char *)src + n;
        if ((long)dest & 1) {
            char *cdest = dest;
            const char *csrc = src;
            *--cdest = *--csrc;
            dest = cdest;
            src = csrc;
            n--;
        }
        if (n > 2 && (long)dest & 2) {
            short *sdest = dest;
            const short *ssrc = src;
            *--sdest = *--ssrc;
            dest = sdest;
            src = ssrc;
            n -= 2;
        }
        temp = n >> 2;
        if (temp) {
            long *ldest = dest;
            const long *lsrc = src;
            temp--;
            do
                *--ldest = *--lsrc;
            while (temp--);
            dest = ldest;
            src = lsrc;
        }
        if (n & 2) {
            short *sdest = dest;
            const short *ssrc = src;
            *--sdest = *--ssrc;
            dest = sdest;
            src = ssrc;
        }
        if (n & 1) {
            char *cdest = dest;
            const char *csrc = src;
            *--cdest = *--csrc;
        }
    }
    return xdest;
}
