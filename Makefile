CC=gcc
CFLAGS= -g -Wall -Wextra
LIBS=

SRCS=$(wildcard *.c)
TARGETS=$(SRCS:.c=)

.PHONY: all clean

all: $(TARGETS)

debug: CFLAGS += -Wextra -g -DDEBUG=1
debug: $(TARGETS)

clean:
	rm -f $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)