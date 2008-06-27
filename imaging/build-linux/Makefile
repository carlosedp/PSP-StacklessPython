# -*- makefile -*-

TARGET_LIB = libimaging.a

OBJS = ../src/Exception.o \
	../src/Drawable.o \
	../src/Image.o \
	../src/Blitter.o \
	../src/Font.o \
	../src/Mask.o \
	../src/MorphFilters.o \
	../src/ColourFilters.o \
	../src/MiscFilters.o

CXXFLAGS = -O3 -Wall -march=i686 -mmmx
CPPFLAGS = -I.. -DLINUX

all: $(TARGET_LIB)

$(TARGET_LIB): $(OBJS)
	$(AR) rcs $(TARGET_LIB) $(OBJS)

clean:
	rm -f $(TARGET_LIB) $(OBJS) *~

doc:
	cd ..; doxygen
