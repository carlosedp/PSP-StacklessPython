
all: libs

libs:
	cd libpsp2d && make
	cd libpspsnd && make

clean:
	cd libpsp2d && make clean
	cd libpspsnd && make clean

install:
	cd libpsp2d && make install
	cd libpspsnd && make install

doc:
	cd libpsp2d && make doc
	cd libpspsnd && make doc
