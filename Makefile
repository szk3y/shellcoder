SRCS = main.c
ASRCS64 = caller64.asm
ASRCS32 = caller32.asm
AOBJS64 = $(ASRCS64:.asm=.o)
AOBJS32 = $(ASRCS32:.asm=.o)
AOBJS = $(AOBJS64) $(AOBJS32)
TARGET64 = shell_loader64
TARGET32 = shell_loader32
TARGET = $(TARGET64) $(TARGET32)

CC = gcc
CFLAGS = -g -O0 -Wall -Wextra -std=c11
LIBS =
ASM = nasm
A32FLAGS = -f elf32
A64FLAGS = -f elf64

.PHONY: all
all: $(AOBJS)
	$(MAKE) -C test
	$(CC) -o $(TARGET64) $(CFLAGS) $(SRCS) $(AOBJS64) $(LIBS)
	$(CC) -o $(TARGET32) -m32 $(CFLAGS) $(SRCS) $(AOBJS32) $(LIBS)

caller64.o: caller64.asm
	$(ASM) -o $@ $(A64FLAGS) $<

caller32.o: caller32.asm
	$(ASM) -o $@ $(A32FLAGS) $<

.PHONY: clean
clean:
	$(MAKE) clean -C test
	$(RM) $(TARGET) $(AOBJS)
