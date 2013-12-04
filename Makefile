# Makefile for chatd

CFLAGS = -Wall
SOURCES = main.c
OBJS = $(SOURCES:.c=.o)

all: clean chatd

chatd: $(OBJS)
	cc $(OBJS) -o chatd

clean:
	rm -f *o chatd