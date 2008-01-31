TARGET = hello
OBJS = main.o menu.o

INCDIR =
CFLAGS = -G0 -Wall
CPPFLAGS = -I../..
CXXFLAGS = $(CFLAGS) -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS = -L../..
LIBS = -lpspsnd -lpsp2d -lmikmod -lmmio -lpspaudiolib -lpspaudio -lpspgu -lpng -ljpeg -lz -lm -lstdc++

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Hello World

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
