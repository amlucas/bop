M=../make
BOP=../bop

include $M/common.mk

LIBS      = -lbop
LDFLAGS   = -L$(BOP)
CXXFLAGS += -I$(BOP)

PROGS = bop2vtk bop2txt

all: $(PROGS)

%: %.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

bop2vtk.o: bop2vtk.cpp
bop2txt.o: bop2txt.cpp

install:
	mkdir -p $(INST_BIN)
	cp $(PROGS) $(INST_BIN)

clean:
	rm -f $(PROGS) *.o

.PHONY: clean install
