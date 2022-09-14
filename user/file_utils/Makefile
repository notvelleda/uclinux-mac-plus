#BASEDIR=..

#include $(BASEDIR)/Make.defs

###############################################################################
#
# Include standard packaging commands.

#include $(BASEDIR)/Make.rules

###############################################################################

# note: grep doesn't read stdin, replaced with minix1/grep
# removed: l
# uclinux: removed df, grep
PRGS = ln ls mkdir mkfifo mknod more mv rm rmdir sync touch \
	cat chgrp chmod chown cmp cp dd split

all: $(PRGS)

cat: cat.o
	$(CC) $(LDFLAGS) -o cat cat.o $(LDLIBS)

chgrp: chgrp.o
	$(CC) $(LDFLAGS) -o chgrp chgrp.o $(LDLIBS)

chmod: chmod.o
	$(CC) $(LDFLAGS) -o chmod chmod.o $(LDLIBS)

chown: chown.o
	$(CC) $(LDFLAGS) -o chown chown.o $(LDLIBS)

cmp: cmp.o
	$(CC) $(LDFLAGS) -o cmp cmp.o $(LDLIBS)

cp: cp.o $(TINYPRINTF)
	$(CC) $(LDFLAGS) -maout-heap=0xffff -o cp cp.o $(TINYPRINTF) $(LDLIBS)

df: df.o $(TINYPRINTF)
	$(CC) $(LDFLAGS) -o df df.o $(TINYPRINTF) $(LDLIBS)

dd: dd.o
	$(CC) $(LDFLAGS) -maout-heap=12288 -o dd dd.o $(LDLIBS)

grep: grep.o
	$(CC) $(LDFLAGS) -o grep grep.o $(LDLIBS)

l: l.o
	$(CC) $(LDFLAGS) -o l l.o $(LDLIBS)

ln: ln.o
	$(CC) $(LDFLAGS) -o ln ln.o $(LDLIBS)

ls: ls.o
	$(CC) $(LDFLAGS) -maout-heap=20480 -o ls ls.o $(LDLIBS)

mkdir: mkdir.o
	$(CC) $(LDFLAGS) -o mkdir mkdir.o $(LDLIBS)

mkfifo: mkfifo.o
	$(CC) $(LDFLAGS) -o mkfifo mkfifo.o $(LDLIBS)

mknod: mknod.o
	$(CC) $(LDFLAGS) -o mknod mknod.o $(LDLIBS)

more: more.o
	$(CC) $(LDFLAGS) -o more more.o $(LDLIBS)

mv: mv.o
	$(CC) $(LDFLAGS) -o mv mv.o $(LDLIBS)

rm: rm.o $(TINYPRINTF)
	$(CC) $(LDFLAGS) -maout-stack=12228 -o rm rm.o $(TINYPRINTF) $(LDLIBS)

rmdir: rmdir.o
	$(CC) $(LDFLAGS) -o rmdir rmdir.o $(LDLIBS)

split: split.o
	$(CC) $(LDFLAGS) -o split split.o $(LDLIBS)

sync: sync.o
	$(CC) $(LDFLAGS) -o sync sync.o $(LDLIBS)

touch: touch.o
	$(CC) $(LDFLAGS) -o touch touch.o $(LDLIBS)

install: $(PRGS)
	$(INSTALL) $(PRGS) $(DESTDIR)/bin

clean:
	rm -f $(PRGS) *.o
