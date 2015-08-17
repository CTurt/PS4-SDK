CC		:=	gcc
ODIR	:=	build
SDIR	:=	source
IDIR	:=	include
LDIR	:=	lib
CFLAGS	:=	-I$(IDIR) -O2 -nostartfiles -nostdlib -Wall -masm=intel -march=x86-64 -m64
LFLAGS	:=	-L$(LDIR) -fPIC
CFILES	:=	$(wildcard $(SDIR)/*.c)
OBJS	:=	$(patsubst $(SDIR)/%.c, build/%.o, $(wildcard $(SDIR)/*.c))

PLATFORM = $(shell uname)

ifeq ($(findstring Linux,$(PLATFORM)), Linux)
	TARGET=$(shell basename $(CURDIR))
endif

ifeq ($(findstring Darwin,$(PLATFORM)), Darwin)
	TARGET=$(shell basename $(CURDIR))
endif

ifeq ($(findstring MINGW,$(PLATFORM)), MINGW)
	TARGET=$(shell basename $(CURDIR)).exe
endif

$(TARGET): $(ODIR) $(OBJS)
	$(CC) $(ODIR)/*.o -o $(TARGET) $(CFLAGS) $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS) 

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	rm -f $(TARGET) $(ODIR)/*.o
