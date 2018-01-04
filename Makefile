all: converters utils libbop install installtools tests

install: libbop
	make -C src install

installtools: converters
	make -C converters install

libbop: ; make -C src/

utils:      libbop install ;  make -C utils/
converters: libbop install ;  make -C converters/
tests:      libbop install ;  make -C test/

test: all
	make -C utils test
	make -C converters test
	make -C test test

clean:
	make -C src clean
	make -C utils clean
	make -C converters clean
	make -C test clean

.PHONY: clean install test tests libbop utils converters
