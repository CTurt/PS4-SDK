CC		:=	clang
AS		:=	clang
AR		:=	ar
OBJCOPY	:=	objcopy
ODIR	:=	build
SDIR	:=	source
IDIR	:=	include
LDIR	:=	lib
CFLAGS	:=	-I$(IDIR) -O2 -std=c11 -ffreestanding -fno-builtin -Wall -target x86_64-scei-ps4-elf -masm=intel -m64 -mcmodel=large
SFLAGS	:=	-target x86_64-scei-ps4-elf
CFILES	:=	$(wildcard $(SDIR)/*.c)
SFILES	:=	$(wildcard $(SDIR)/*.s)
OBJS	:=	$(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(SFILES))

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
