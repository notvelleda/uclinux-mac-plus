EXEC = stage2.bin
OBJS = stage2.o

# need UCLINUX_DIR

CFLAGS := -I. -m68000 -Os -fomit-frame-pointer -nostdlib
LDFLAGS := -T stage2.ld
#CC := cc

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o
