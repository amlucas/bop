all: converters utils libbop

install: converters libbop
	make -C bop install
	make -C converters install

libbop: ; make -C bop/

utils:      libbop ;  make -C utils/
converters: libbop ;  make -C converters/

test: all
	make -C utils/ test
	make -C converters/ test

clean:
	make -C bop clean
	make -C utils clean
	make -C converters clean

.PHONY: clean install test libbop utils converters
