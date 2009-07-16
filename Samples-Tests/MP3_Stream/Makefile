TARGET = mp3stream
OBJS = MusicEngine.o main.o

BUILD_PRX=1
PSP_FW_VERSION = 200
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBS = -lmad -lpspaudiolib -lpspaudio -lpsppower
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = MP3 Streaming

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
