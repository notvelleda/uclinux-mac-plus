all: ll
ll: ll.o
	$(CC) $(LDFLAGS) -o $@ ll.o $(LDLIBS)
%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $*.o
