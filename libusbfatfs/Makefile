LIBPS4  := $(PS4SDK)/libPS4

CC      := clang
AS      := clang
AR      := ar
OBJCOPY := objcopy
ODIR    := build
SDIR    := source
IDIRS   := -I$(LIBPS4)/include -I. -Iinclude
CFLAGS	:= $(IDIRS) -O2 -std=c11 -ffreestanding -fno-builtin -nostdlib -Wall -target x86_64-scei-ps4-elf -masm=intel -march=btver2 -mtune=btver2 -m64 -mcmodel=large
SFLAGS	:= -nostartfiles -nostdlib -target x86_64-scei-ps4-elf -march=btver2 -mtune=btver2
CFILES  := $(wildcard $(SDIR)/*.c)
SFILES  := $(wildcard $(SDIR)/*.s)
OBJS	:= $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(SFILES))

TARGET = $(shell basename $(CURDIR)).a

$(TARGET): $(ODIR) $(OBJS)
	$(AR) rcs $@ $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	$(AS) -c -o $@ $< $(SFLAGS)

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	rm -rf $(ODIR) $(TARGET) $(ODIR)/*.o
