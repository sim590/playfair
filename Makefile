.PHONY: clean, mrproper
CC = gcc
CFLAGS = -g -Wall

playfair_CONFIG = config.h
playfair_SOURCES = playfair.c cipher.c shelper.c

all: debug playfair

debug:
	sed 's/\(#define DEBUG \).*/\1$(if $(DEBUG),1,0)/' $(playfair_CONFIG).in >$(playfair_CONFIG)

playfair: $(playfair_SOURCES:.c=.o)
	$(CC) $(CFLAGS) -o $@ $+

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o core.*

mrproper: clean
	rm -f playfair

#  vim: set sts=0 ts=4 sw=4 tw=120 noet :

