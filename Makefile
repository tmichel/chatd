# Makefile for chatd

SRCDIR = src
OBJDIR = obj

CFLAGS = -Wall -pedantic -std=c99
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(OBJDIR) chatd

chatd: $(OBJS)
	cc $(OBJS) -o chatd

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	cc $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	-rm -f chatd $(OBJS)
