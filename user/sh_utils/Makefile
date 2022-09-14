#BASEDIR=..

#include $(BASEDIR)/Make.defs

###############################################################################
#
# Include standard packaging commands.

#include $(BASEDIR)/Make.rules

###############################################################################

# TODO: write	# Do not compile
PRGS=basename clear date dirname echo false printenv pwd true which whoami \
	yes logname tr xargs mesg stty test uname

write: write.o ../sys_utils/utent.o

all: $(PRGS)

basename: basename.o
	$(CC) $(LDFLAGS) -o basename basename.o $(LDLIBS)

clear: clear.o
	$(CC) $(LDFLAGS) -o clear clear.o $(LDLIBS)

date: date.o
	$(CC) $(LDFLAGS) -o date date.o $(LDLIBS)

dirname: dirname.o
	$(CC) $(LDFLAGS) -o dirname dirname.o $(LDLIBS)

echo: echo.o
	$(CC) $(LDFLAGS) -o echo echo.o $(LDLIBS)

false: false.o
	$(CC) $(LDFLAGS) -o false false.o $(LDLIBS)

printenv: printenv.o
	$(CC) $(LDFLAGS) -o printenv printenv.o $(LDLIBS)

pwd: pwd.o
	$(CC) $(LDFLAGS) -o pwd pwd.o $(LDLIBS)

true: true.o
	$(CC) $(LDFLAGS) -o true true.o $(LDLIBS)

which: which.o $(TINYPRINTF)
	$(CC) $(LDFLAGS) -o which which.o $(TINYPRINTF) $(LDLIBS)

whoami: whoami.o
	$(CC) $(LDFLAGS) -o whoami whoami.o $(LDLIBS)

yes: yes.o
	$(CC) $(LDFLAGS) -o yes yes.o $(LDLIBS)

logname: logname.o
	$(CC) $(LDFLAGS) -o logname logname.o $(LDLIBS)

tr: tr.o
	$(CC) $(LDFLAGS) -o tr tr.o $(LDLIBS)

xargs: xargs.o
	$(CC) $(LDFLAGS) -o xargs xargs.o $(LDLIBS)

mesg: mesg.o $(TINYPRINTF)
	$(CC) $(LDFLAGS) -o mesg mesg.o $(TINYPRINTF) $(LDLIBS)

stty: stty.o
	$(CC) $(LDFLAGS) -o stty stty.o $(LDLIBS)

test: test.o
	$(CC) $(LDFLAGS) -o test test.o $(LDLIBS)

uname: uname.o
	$(CC) $(LDFLAGS) -o uname uname.o $(LDLIBS)

install: $(PRGS)
	$(INSTALL) $(PRGS) $(DESTDIR)/bin

clean:
	rm -f *.o $(PRGS)
