# Makefile for chatd

CFLAGS = -Wall -pedantic -std=c99
SOURCES = main.c server.c
OBJS = $(SOURCES:.c=.o)

all: chatd

chatd: $(OBJS)
	cc $(OBJS) -o chatd

.PHONY: clean
clean:
	-rm -f chatd $(OBJS)
