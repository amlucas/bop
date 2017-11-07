M=../make
BOP=../bop

include $M/common.mk
include $M/rules.mk

LIBS      = -lbop
LDFLAGS   = -L$(BOP)
CXXFLAGS += -I$(BOP)

PROGS = bop2vtk bop2txt

all: $(PROGS)

test: all
	atest *.cpp

bop2vtk.o: bop2vtk.cpp
bop2txt.o: bop2txt.cpp

install:
	mkdir -p $(INST_BIN)
	cp $(PROGS) $(INST_BIN)

clean:
	rm -f $(PROGS) *.o

.PHONY: test clean install
