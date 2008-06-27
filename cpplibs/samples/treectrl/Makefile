TARGET = treectrl
OBJS = main.o FontFactory.o TreeItem.o TreeCtrl.o

INCDIR = ../.. ../../../imaging
CFLAGS = -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = ../.. ../../../imaging/build-psp
LDFLAGS =
LIBS= -limaging -lpsp2d -ljpeg -lpng -lz -lpspgum -lpspgu -lm -lstdc++

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = TreeCtrl Sample

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
