
all: doc

doc:
	epydoc -v --no-private --no-sourcecode -o html psp2d.py pspsnd.py pspnet.py pspos.py

clean:
	rm -rf html

release: clean doc
	cp -a html/* ~/public_html/pspdoc/
