# Makefile for chatd

SRCDIR = src
OBJDIR = obj
TESTDIR = test
TESTOUTDIR = test_ex

CFLAGS = -Wall -pedantic -std=gnu99 -pthread
LIBS = -lsqlite3
SOURCES = $(wildcard $(SRCDIR)/*.c)
SOURCES := $(filter-out src/main.c, $(SOURCES))
TESTSRC = $(wildcard $(TESTDIR)/*_test.c)
OBJS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TEST_O = $(TESTSRC:$(TESTDIR)/%.c=$(TESTOUTDIR)/%)

all: $(OBJDIR) chatd

chatd: $(OBJS) obj/main.o
	cc $(CFLAGS) $(OBJS) obj/main.o $(LIBS) -o chatd

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	cc $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

obj/main.o:
	cc $(CFLAGS) -c src/main.c -o obj/main.o

.PHONY: clean
clean:
	-rm -rf $(OBJDIR)
	-rm -rf $(TESTOUTDIR)

debug: clean
	cc $(CFLAGS) -g -o chatd src/*.c

# for testing

$(TESTOUTDIR):
	@mkdir -p $(TESTOUTDIR)

$(TEST_O): $(TESTOUTDIR)/% : $(TESTDIR)/%.c
	cc $(CFLAGS) $(OBJS) test/assert.c $< $(LIBS) -o $@

test: test-compile test-run

test-compile: $(OBJDIR) $(TESTOUTDIR) $(OBJS) $(TEST_O)

test-run:
	@$(foreach t,$(shell ls $(TESTOUTDIR)), echo "$(t):"; ./$(TESTOUTDIR)/$(t); echo;)
