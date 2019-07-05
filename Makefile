CC = cc
CFLAGS = -Wall -O2
LDFLAGS =

all: test1d testseg slow fast

%.o: %.c
	$(CC) -c $(CFLAGS) $<
slow: slow.o
	$(CC) -o $@ $^ $(LDFLAGS)
fast: fast.o seg.o
	$(CC) -o $@ $^ $(LDFLAGS)
test1d: test1d.o
	$(CC) -o $@ $^ $(LDFLAGS)
testseg: testseg.o seg.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o test1d testseg
