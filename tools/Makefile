M=../make
include $M/common.mk
include $M/rules.mk

BOP=../src

LIBS      = -lbop -lbop_serial
LDFLAGS   = -L$(BOP)
CXXFLAGS += -I$(BOP)

PROGS = paste

all: $(PROGS)

test: all
	atest paste.cpp

paste.o: paste.cpp

install:
	mkdir -p $(INST_BIN)
	cp $(PROGS) $(INST_BIN)

clean: ; rm -f $(PROGS) *.o

.PHONY: clean test
