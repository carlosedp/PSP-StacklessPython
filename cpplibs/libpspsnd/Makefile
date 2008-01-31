
PSPSDK=$(shell psp-config --pspsdk-path)
PSPDIR=$(shell psp-config --psp-prefix)

TARGET_LIB = ../libpspsnd.a

OBJS = Sound.o Music.o

CXXFLAGS = -Os -G0 -Wall
CPPFLAGS = -I..

include $(PSPSDK)/lib/build.mak

install: $(TARGET_LIB)
	@echo "Installing libpspsnd into $(PSPDIR)"
	@mkdir -p $(PSPDIR)/include/libpspsnd $(PSPDIR)/lib
	@cp *.h $(PSPDIR)/include/libpspsnd
	@cp libpspsnd.a  $(PSPDIR)/lib
	@echo "Done"

doc:
	doxygen
