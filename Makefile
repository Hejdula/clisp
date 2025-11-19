
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
CPPFLAGS = -Isrc -Iinclude -MMD -MP
TARGET = main

BINDIR := bin
OBJDIR := obj
SRCDIR := src
DOCSDIR := docs
SUBDIR := submission
INCLDIR := include

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(BINDIR)/$(TARGET)
# 	./$(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) $(BINDIR)

submission: all Makefile Makefile.win
	rm -rf $(SUBDIR)
	mkdir $(SUBDIR)
	mkdir $(SUBDIR)/$(SRCDIR)
	cp $(DOCSDIR)/clisp.pdf $(SUBDIR)/clisp.pdf
	cp Makefile $(SUBDIR)/Makefile
	cp Makefile.win $(SUBDIR)/Makefile.win
	cp $(SRCDIR)/* $(SUBDIR)/$(SRCDIR)/
	cp $(INCLDIR)/* $(SUBDIR)/$(SRCDIR)/
	zip -r $(SUBDIR).zip $(SUBDIR)
	
-include $(DEPS)