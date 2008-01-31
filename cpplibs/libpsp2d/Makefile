
PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR=$(shell psp-config --psp-prefix)

TARGET_LIB = ../libpsp2d.a

OBJS = framebuffer.o \
	Exception.o \
	Drawable.o \
	Screen.o \
	Image.o \
	Blitter.o \
	Controller.o \
	Font.o \
	Mask.o \
	BlitBatch.o \
	Timer.o

CXXFLAGS = -Os -G0 -Wall
CPPFLAGS = -I..

include $(PSPSDK)/lib/build.mak

install: $(TARGET_LIB)
	@echo "Installing libpsp2d into $(PSPDIR)"
	@mkdir -p $(PSPDIR)/include/libpsp2d $(PSPDIR)/lib
	@cp *.h $(PSPDIR)/include/libpsp2d
	@cp *.il $(PSPDIR)/include/libpsp2d
	@cp libpsp2d.a  $(PSPDIR)/lib
	@echo "Done"

doc:
	doxygen
