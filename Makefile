
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
TARGET = main

BINDIR := bin
OBJDIR := obj
SRCDIR := src

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

.PHONY: all clean

all: $(BINDIR)/$(TARGET)
# 	./$(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) $(BINDIR)
