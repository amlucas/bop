all: converters utils tools libbop install installtools tests

install: libbop
	make -C src install

installtools: converters tools
	make -C converters install	
	make -C tools      install

libbop: ; make -C src/

utils:      libbop install ;  make -C utils/
converters: libbop install ;  make -C converters/
converters: libbop install ;  make -C tools/
tests:      libbop install ;  make -C test/

test: all
	make -C utils test
	make -C converters test
	make -C tools test
	make -C test test

clean:
	make -C src clean
	make -C utils clean
	make -C converters clean
	make -C tools clean
	make -C test clean

.PHONY: clean install test tests libbop utils converters tools
