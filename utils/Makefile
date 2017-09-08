M=../make
include $M/common.mk

BOP=../bop

LIBS      = -lbop
LDFLAGS   = -L$(BOP)
CXXFLAGS += -I$(BOP)

PROGS = seq2bop rwascii

all: $(PROGS)

%: %.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

seq2bop.o: seq2bop.cpp
rwascii.o: rwascii.cpp

install:
	mkdir -p $(INST_BIN)
	cp $(PROGS) $(INST_BIN)

clean: ; rm -f $(PROGS) *.o

.PHONY: clean
