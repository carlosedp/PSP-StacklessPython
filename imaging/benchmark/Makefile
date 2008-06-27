
TARGET=bench
OBJS=test.o

CXXFLAGS=-O3 -Wall -march=i686 -mmmx
CPPFLAGS=-I.. -DLINUX
LDFLAGS=-L../build-linux
LIBS=-limaging -lpng -lz -ljpeg

CXX=g++

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS) *~

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)
