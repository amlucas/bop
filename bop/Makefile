M=../make
include $M/common.mk
include $M/rules.mk

libbop.a: bop_reader.o bop_writer.o bop_common.o
	ar rcs $@ $^

bop_reader.o: bop_reader.cpp bop_reader.h bop_macros.h bop_common.h
bop_writer.o: bop_writer.cpp bop_writer.h bop_macros.h bop_common.h
bop_common.o: bop_common.cpp bop_common.h bop_macros.h


.PHONY: clean install
clean:; rm -rf *.o *.a

install:
	mkdir -p $(INST_BIN)
	mkdir -p $(INST_LIB)/include
	mkdir -p $(INST_LIB)/lib
	cp *.h $(INST_LIB)/include/
	cp *.a $(INST_LIB)/lib/

