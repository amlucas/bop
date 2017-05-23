BIN = $(HOME)/bin # where to install
CXXFLAGS = -std=c++11 -Wpedantic -Wall -O3
CXX=g++

PROGS = bop2vtk bop2txt

LIBS      = -lbop
LDFLAGS   = -Lbop
CXXFLAGS += -Ibop

all: $(PROGS)
%: %.o; $(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

bop2txt.o: bop2txt.cpp libbop
bop2vtk.o: bop2vtk.cpp libbop

install: all
	mkdir -p $(BIN)
	cp $(PROGS) $(BIN)

libbop:; make -C bop/

clean:
	rm -f $(PROGS) *.o
	make -C bop clean

.PHONY: clean install libbop
