SRCS = main.c
TARGET_64BIT = shellcoder_64bit
TARGET_32BIT = shellcoder_32bit
TARGET = $(TARGET_64BIT) $(TARGET_32BIT)

CC = gcc
CFLAGS = -g -O0 -Wall -Wextra -std=gnu11
LIBS =

.PHONY: all
all: $(AOBJS)
	$(MAKE) -C test
	$(CC) -o $(TARGET_64BIT) $(CFLAGS) $(SRCS) $(LIBS)
	$(CC) -o $(TARGET_32BIT) -m32 $(CFLAGS) $(SRCS) $(LIBS)

.PHONY: clean
clean:
	$(MAKE) clean -C test
	$(RM) $(TARGET)
