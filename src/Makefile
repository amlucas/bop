M=../make
include $M/common.mk
include $M/rules.mk

inc=bop_common.h bop_serial.h
lib=libbop.a

libbop.a: bop_common.o utils.o header.o bop_serial.o
	ar rcs $@ $^

bop_common.o: bop_common.cpp bop_common.h macros.h utils.h
bop_serial.o: bop_serial.cpp bop_serial.h macros.h utils.h header.h 

utils.o:  utils.cpp utils.h bop_common.h
header.o: header.cpp header.h bop_common.h utils.h macros.h

.PHONY: clean install
clean:; rm -rf *.o *.a

install:
	mkdir -p $(INST_BIN)
	mkdir -p $(INST_LIB)/include
	mkdir -p $(INST_LIB)/lib
	cp $(inc) $(INST_LIB)/include/
	cp $(lib) $(INST_LIB)/lib/

