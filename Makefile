CC = cc
CFLAGS = -Wall -O2
LDFLAGS =

all: test1d testseg slow fast test2d tdrive slowx

%.o: %.c
	$(CC) -c $(CFLAGS) $<
slow: slow.o
	$(CC) -o $@ $^ $(LDFLAGS) -lm
fast: fast.o seg.o
	$(CC) -o $@ $^ $(LDFLAGS)
slowx: slowx.o
	$(CC) -o $@ $^ $(LDFLAGS)
test1d: test1d.o
	$(CC) -o $@ $^ $(LDFLAGS)
test2d: test2d.o
	$(CC) -o $@ $^ $(LDFLAGS)
testseg: testseg.o seg.o
	$(CC) -o $@ $^ $(LDFLAGS)
tdrive: tdrive.o
	$(CC) -o $@ $^ $(LDFLAGS) -lm

clean:
	rm -f *.o test1d testseg
