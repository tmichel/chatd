# Makefile for chatd

CFLAGS = -c -Wall
SOURCES = main.c
OBJS = $(SOURCES:.c=.o)

all: chatd

chatd: $(OBJS)
	gcc $(OBJS) -o chatd

main.o: main.c
	gcc $(CFLAGS) main.c

clean:
	rm -f *o chatd