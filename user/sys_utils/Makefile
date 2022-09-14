#BASEDIR=..

#include $(BASEDIR)/Make.defs

###############################################################################
#
# Include standard packaging commands.

#include $(BASEDIR)/Make.rules

###############################################################################

#ELKS_LIB=$(ELKS_DIR)/arch/i86/lib

# clock enabled and has direct I/O port access
# knl, insmod removed as useless
# exitemu disabled because it calls INT directly to DOSEMU
# uclinux: disabled unreal(16), init, clock, (u)mount, makeboot, chmem, mouse, ps, meminfo, console, shutdown, poweroff, reboot
# uclinux: disabled login, passwd, getty (tinylogin replaces them)
PRGS = \
	kill \
	who \
	man \
	sercat \
	# EOL

init: init.o
	$(CC) $(LDFLAGS) -maout-heap=4096 -maout-stack=1024 -o init init.o $(LDLIBS)

getty: getty.o $(TINYPRINTF)
	$(CC) $(LDFLAGS) -maout-heap=1024 -maout-stack=1024 -o getty getty.o $(TINYPRINTF) $(LDLIBS)

login: login.o
	$(CC) $(LDFLAGS) -maout-stack=2048 -o login login.o $(LDLIBS)

kill: kill.o
	$(CC) $(LDFLAGS) -o kill kill.o $(LDLIBS)

mount: mount.o $(TINYPRINTF)
	$(CC) $(LDFLAGS) -o mount mount.o $(TINYPRINTF) $(LDLIBS)

umount: umount.o
	$(CC) $(LDFLAGS) -o umount umount.o $(LDLIBS)

passwd: passwd.o
	$(CC) $(LDFLAGS) -o passwd passwd.o $(LDLIBS)

reboot: reboot.o
	$(CC) $(LDFLAGS) -o reboot reboot.o $(LDLIBS)

shutdown: shutdown.o
	$(CC) $(LDFLAGS) -o shutdown shutdown.o $(LDLIBS)

ps: ps.o $(TINYPRINTF)
	$(CC) $(LDFLAGS) -maout-heap=1024 -maout-stack=2048 -o ps ps.o $(TINYPRINTF) $(LDLIBS)

meminfo: meminfo.o $(TINYPRINTF)
	$(CC) $(LDFLAGS) -maout-heap=1 -maout-stack=512 -o meminfo meminfo.o $(TINYPRINTF) $(LDLIBS)

who: who.o
	$(CC) $(LDFLAGS) -o who who.o $(LDLIBS)

clock: clock.o
	$(CC) $(LDFLAGS) -o clock clock.o $(LDLIBS)

exitemu: exitemu.o
	$(CC) $(LDFLAGS) -o exitemu exitemu.o $(LDLIBS)

knl: knl.o
	$(CC) $(LDFLAGS) -o knl knl.o $(LDLIBS)

man: man.o
	$(CC) $(LDFLAGS) -o man man.o $(LDLIBS)

poweroff: poweroff.o
	$(CC) $(LDFLAGS) -o poweroff poweroff.o $(LDLIBS)

chmem: chmem.o $(TINYPRINTF)
	$(CC) $(LDFLAGS) -o chmem chmem.o $(TINYPRINTF) $(LDLIBS)

mouse: mouse.o $(TINYPRINTF)
	$(CC) $(LDFLAGS) -o mouse mouse.o $(TINYPRINTF) $(LDLIBS)

sercat: sercat.o
	$(CC) $(LDFLAGS) -o sercat sercat.o $(LDLIBS)

console: console.o
	$(CC) $(LDFLAGS) -o console console.o $(LDLIBS)

makeboot: makeboot.o $(TINYPRINTF)
	$(CC) $(LDFLAGS) -o makeboot makeboot.o $(TINYPRINTF) $(LDLIBS)

unreal.o: $(ELKS_LIB)/unreal.S
	$(LD) -melks-libc -mcmodel=small -c $(ELKS_LIB)/unreal.S -o unreal.o

unreal16: unreal16.o unreal.o
	$(LD) -melks-libc -mcmodel=small -c unreal16.S -o unreal16.o
	$(LD) -melks-libc -mcmodel=small -nostdlib -o unreal16 unreal16.o unreal.o

all: $(PRGS)

install: $(PRGS)
	$(INSTALL) $(PRGS) $(DESTDIR)/bin

clean:
	rm -f *.o core $(PRGS)
