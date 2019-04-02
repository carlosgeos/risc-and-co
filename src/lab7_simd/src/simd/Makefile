all: simd
SRC=./
CC=gcc
CFLAGS=-g -ggdb -Wall -Werror -Wextra -Wshadow -std=gnu11 -D _GNU_SOURCE -msse3 -lrt
LDFLAGS=-lm

FILES=$(wildcard ${SRC}*.c)
OBJS=$(subst .c,.o,$(FILES))

%.o: %.c
		$(CC) -c $(CFLAGS) -o $@ $<

sisd:	$(OBJS)
		$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
		rm -f *.o
		rm simd

print-%  : ; @echo $* = $($*)
