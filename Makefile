CC = cc
CFLAGS = -Wall -O2
LDFLAGS =

all: test1d

%.o: %.c
	$(CC) -c $(CFLAGS) $<
test1d: test1d.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o test1d
