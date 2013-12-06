# Makefile for chatd

SRCDIR = src
OBJDIR = obj

CFLAGS = -Wall -pedantic -std=gnu99
SOURCES = $(wildcard $(SRCDIR)/*.c)
SOURCES := $(filter-out src/main.c, $(SOURCES))
OBJS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(OBJDIR) chatd

chatd: $(OBJS) obj/main.o
	cc $(OBJS) obj/main.o -o chatd

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	cc $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

obj/main.o:
	cc $(CFLAGS) -c src/main.c -o obj/main.o

.PHONY: clean
clean:
	-rm -f chatd $(OBJS) obj/main.o
	-rm -f run_test

test: $(OBJS)
	ruby ./test/lib/gen.rb
	cc -std=c99 -o run_test $(OBJS) test/*.c test/lib/*.c
	./run_test