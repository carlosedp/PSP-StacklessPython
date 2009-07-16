TARGET = hello
OBJS = main.o

CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBS = -lmad -lpspaudiolib -lpspaudio -lpsppower -lpspaudiocodec
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = MP3 Test

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
