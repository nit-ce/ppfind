CC = cc
CFLAGS = -Wall -O2
LDFLAGS =

all: test1d slow

%.o: %.c
	$(CC) -c $(CFLAGS) $<
slow: slow.o
	$(CC) -o $@ $^ $(LDFLAGS)
test1d: test1d.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o test1d
