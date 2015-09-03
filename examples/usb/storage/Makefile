LIBPS4	:=	$(PS4SDK)/libPS4

CC		:=	clang
AS		:=	clang
LD		:=	ld.gold
OBJCOPY	:=	objcopy
ODIR	:=	build
SDIR	:=	source
IDIRS	:=	-I$(LIBPS4)/include -I. -Iinclude
LDIRS	:=	-L$(LIBPS4) -L. -Llib
CFLAGS	:=	$(IDIRS) -O2 -fPIC -ffreestanding -fno-builtin -nostdlib -Wall -masm=intel -target x86_64-scei-ps4-elf -march=btver2 -mtune=btver2 -m64 -mcmodel=large
SFLAGS	:=	-nostartfiles -nostdlib -target x86_64-scei-ps4-elf -march=btver2 -mtune=btver2
LFLAGS	:=	$(LDIRS) -shared -fPIE
CFILES	:=	$(wildcard $(SDIR)/*.c)
SFILES	:=	$(wildcard $(SDIR)/*.s)
OBJS	:=	$(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(SFILES))

LIBS	:=	-lPS4

TARGET = $(shell basename $(CURDIR))

#$(TARGET).bin: $(TARGET).elf
#	$(OBJCOPY) -O binary $^ $@

$(TARGET).elf: $(ODIR) $(OBJS)
	$(LD) $(ODIR)/*.o -o $@ $(LFLAGS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	$(AS) -c -o $@ $< $(SFLAGS)

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	rm -f $(TARGET) $(ODIR)/*.o
