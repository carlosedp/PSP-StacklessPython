
PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR=$(shell psp-config --psp-prefix)

TARGET_LIB = libimaging.a

OBJS = ../src/framebuffer.o \
	../src/Exception.o \
	../src/Drawable.o \
	../src/Screen.o \
	../src/Image.o \
	../src/Blitter.o \
	../src/Font.o \
	../src/Mask.o \
	../src/MorphFilters.o \
	../src/ColourFilters.o \
	../src/MiscFilters.o

CXXFLAGS = -Os -G0 -Wall -fno-strict-aliasing
CPPFLAGS = -I..

include $(PSPSDK)/lib/build.mak

install: $(TARGET_LIB)
	@echo "Installing libimaging into $(PSPDIR)"
	@mkdir -p $(PSPDIR)/include/libimaging $(PSPDIR)/lib
	@cp *.h $(PSPDIR)/include/libimaging
	@cp *.il $(PSPDIR)/include/libimaging
	@cp libimaging.a  $(PSPDIR)/lib
	@echo "Done"

doc:
	cd ..; doxygen
