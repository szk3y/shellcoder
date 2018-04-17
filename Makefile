SRCS = main.c
TARGET_X64 = shellcoder_x64
TARGET_X86 = shellcoder_x86
TARGET = $(TARGET_X64) $(TARGET_X86)

CC = gcc
CFLAGS = -g -O0 -Wall -Wextra -std=c11
LIBS =

.PHONY: all
all: $(AOBJS)
	$(MAKE) -C test
	$(CC) -o $(TARGET_X64) $(CFLAGS) $(SRCS) $(LIBS)
	$(CC) -o $(TARGET_X86) -m32 $(CFLAGS) $(SRCS) $(LIBS)

.PHONY: clean
clean:
	$(MAKE) clean -C test
	$(RM) $(TARGET)
