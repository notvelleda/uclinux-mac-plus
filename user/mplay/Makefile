all: mplay
mplay: mplay.o
	$(CC) $(LDFLAGS) -o $@ mplay.o $(LDLIBS)
%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $*.o
