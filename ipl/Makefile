# Makefile for iGameGear
#
# Copyright (C) 1998, 1999, 2000  Charles Mac Donald
# iPodLinux/Hotdog port by Luiz "venox" Ribeiro <luizribeiro@gmail.com>
# Based on SDL version by Gregory Montoir <cyx@frenchkiss.net>
#
# Defines :
# LSB_FIRST   : for little endian systems.
# X86_ASM     : enable x86 assembly optimizations
# USE_ZLIB    : enable ZIP file support
# ENABLESOUND : enable sound subsystem

NAME	  = sms

INSTPATH  = /mnt/ipl/home/igg/

CC        = /usr/local/arm-uclinux-tools2/bin/arm-elf-gcc
CFLAGS    = -O3 -funroll-loops -mcpu=arm7tdmi -mtune=arm7tdmi -msoft-float
DEFINES   = -DLSB_FIRST
INCLUDES  = -I. -I.. -I../cpu -I../../hotdog
LIBS	  = -L../../hotdog/ipod -lhotdog -lm

LDFLAGS   = -Wl,-elf2flt

OBJECTS   = main.o saves.o ipl.o input.o video.o \
            ../render.o ../sms.o ../system.o ../vdp.o \
            ../cpu/z80.o

# uncomment to enable sound subsystem (not working yet)
#DEFINES   += -DENABLESOUND
#INCLUDES  += -I../sound
#OBJECTS   += ../sound/sn76496.o ../sound/emu2413.o

# uncomment to enable ZIP support
DEFINES  += -DUSE_ZLIB
INCLUDES += -I../../hotdog/libs/zlib
LIBS     += -L../../hotdog/libs/zlib -lz
OBJECTS  += unzip.o


all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

.c.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) $< -o $@

clean:
	rm -f $(OBJECTS) $(NAME) $(NAME).gdb

install:
	cp $(NAME) $(INSTPATH)
