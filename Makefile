.PHONY: clean, mrproper
CC = gcc
CFLAGS = -g -Wall

playfair_CONFIG = config.h

all: debug playfair

debug:
	sed 's/\(#define DEBUG \).*/\1$(if $(DEBUG),1,0)/' $(playfair_CONFIG).in >$(playfair_CONFIG)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

playfair: playfair.o
	$(CC) $(CFLAGS) -o $@ $+

clean:
	rm -f *.o core.*

mrproper: clean
	rm -f playfair
