LIBPS4	:=	$(PS4SDK)/libPS4

CC		:=	clang
AS		:=	clang
LD		:=	ld.gold
OBJCOPY	:=	objcopy
ODIR	:=	build
SDIR	:=	source
IDIRS	:=	-I$(LIBPS4)/include -I. -Iinclude
LDIRS	:=	-L$(LIBPS4) -L. -Llib
CFLAGS	:=	$(IDIRS) -O2 -fPIC -ffreestanding -fno-builtin -Wall -masm=intel -target x86_64-scei-ps4-elf -m64 -mcmodel=large
SFLAGS	:=	-target x86_64-scei-ps4-elf
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
	rm -rf $(ODIR) $(TARGET) $(ODIR)/*.o
