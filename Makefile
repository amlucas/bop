INST_BIN = $(HOME)/bin # where to install tools
INST_LIB = $(HOME)/prefix/bop

CXXFLAGS = -std=c++11 -Wpedantic -Wall -O3
CXX=g++

PROGS = bop2vtk bop2txt

LIBS      = -lbop
LDFLAGS   = -Lbop
CXXFLAGS += -Ibop

all: $(PROGS) libbop
%: %.o libbop
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

bop2txt.o: bop2txt.cpp
bop2vtk.o: bop2vtk.cpp

install: all
	mkdir -p $(INST_BIN)
	cp $(PROGS) $(INST_BIN)
	mkdir -p $(INST_LIB)/include
	mkdir -p $(INST_LIB)/lib
	cp bop/*.h $(INST_LIB)/include/
	cp bop/*.a $(INST_LIB)/lib/

libbop:
	make -C bop/

clean:
	rm -f $(PROGS) *.o
	make -C bop clean

.PHONY: clean install libbop
