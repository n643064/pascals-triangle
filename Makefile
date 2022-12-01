CC=gcc

CFLAGS=-Wall -O2 $(shell pkg-config --cflags ncurses)
LFLAGS=$(shell pkg-config --libs ncurses)
clean:
	rm -f main.out

main.out: main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)
	./$@ 64
