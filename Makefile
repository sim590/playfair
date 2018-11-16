.PHONY: clean, mrproper
CC = gcc
CFLAGS = -g -Wall

all: debug playfair

debug:
	sed -i 's/\(#define DEBUG \).*/\1$(if $(DEBUG),1,0)/' config.h

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

playfair: playfair.o
	$(CC) $(CFLAGS) -o $@ $+

clean:
	rm -f *.o core.*

mrproper: clean
	rm -f playfair
