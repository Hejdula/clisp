
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
CPPFLAGS = -Isrc -Iinclude -MMD -MP
TARGET = main

BINDIR := bin
OBJDIR := obj
SRCDIR := src

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

-include $(DEPS)