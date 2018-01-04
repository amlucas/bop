M=../make
include $M/common.mk
include $M/rules.mk

BOP=$(INST_LIB)

CXXFLAGS += -I$(BOP)/include
LIBS_B   += -L$(BOP)/lib -lbop

LIBS     = $(LIBS_B) -lbop_serial
LIBS_MPI = $(LIBS_B) -lbop_mpi

PROGS = read write write_mpi

all: $(PROGS)

read.o: read.cpp check.h
write.o: write.cpp check.h

$(MPI_TOK)write_mpi.o: write_mpi.cpp check.h

test:; atest *.cpp

.PHONY: clean test
clean:; rm -rf $(PROGS) *.o
