TARGET = taskcoach
OBJS = main.o \
	../treectrl/FontFactory.o \
	../treectrl/TreeItem.o \
	../treectrl/TreeCtrl.o \
	Task.o \
	TaskFile.o

INCDIR = ../.. ../../../imaging ../treectrl
CFLAGS = -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = ../.. ../../../imaging/build-psp
LDFLAGS =
LIBS= -limaging -lpsp2d -ljpeg -lpng -lz -lpspgum -lpspgu -lm -lstdc++

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = TaskCoach for PSP

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
