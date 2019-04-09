all: filtering
SRC=./
CC=gcc
NO_EXTENDED_IS_FLAGS = -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-ssse3 \
			-mno-sse4.1 -mno-sse4.2 -mno-sse4 -mno-avx -mno-sse4a \
			-mno-fma4 -mno-xop -mno-lwp -mno-3dnow -mno-popcnt

CFLAGS= -g -ggdb -Wall -Werror -Wextra -Wshadow -std=gnu11 -D _GNU_SOURCE
LDFLAGS= -lm

FILES=$(wildcard ${SRC}*.c)
OBJS=$(subst .c,.o,$(FILES))

%.o: %.c
		$(CC) -c $(CFLAGS) -o $@ $<

filtering:	$(OBJS)
		$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
		-rm -f *.o
		-rm filtering

print-%  : ; @echo $* = $($*)
