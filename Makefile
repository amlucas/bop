all: converters utils tools libbop install installtools tests

install: libbop ; (cd src; make install)

installtools: converters tools
	(cd converters; make install)	
	(cd tools;      make install)

libbop: ; (cd src; make)

utils:      libbop install ;  (cd utils;      make)
converters: libbop install ;  (cd converters; make)
tools:      libbop install ;  (cd tools;      make)
tests:      libbop install ;  (cd test;       make)

test: all
	(cd converters; make test)
	(cd test;       make test)
	(cd tools;      make test)
	(cd utils;      make test)

clean:
	(cd converters; make clean)
	(cd src;        make clean)
	(cd test;       make clean)
	(cd utils;      make clean)	
	(cd tools;      make clean)

.PHONY: clean install test tests libbop utils converters tools installtools
