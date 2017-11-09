M=../make
include $M/common.mk
include $M/rules.mk

inc=bop_common.h bop_reader.h bop_writer.h
lib=libbop.a

libbop.a: bop_reader.o bop_writer.o bop_common.o bop_utils.o bop_header.o
	ar rcs $@ $^

bop_reader.o: bop_reader.cpp bop_reader.h bop_macros.h bop_common.h bop_utils.h
bop_writer.o: bop_writer.cpp bop_writer.h bop_macros.h bop_common.h bop_utils.h
bop_common.o: bop_common.cpp bop_common.h bop_macros.h bop_utils.h
bop_utils.o: bop_utils.cpp bop_utils.h bop_common.h
bop_header.o: bop_header.cpp bop_header.h

.PHONY: clean install
clean:; rm -rf *.o *.a

install:
	mkdir -p $(INST_BIN)
	mkdir -p $(INST_LIB)/include
	mkdir -p $(INST_LIB)/lib
	cp $(inc) $(INST_LIB)/include/
	cp $(lib) $(INST_LIB)/lib/

