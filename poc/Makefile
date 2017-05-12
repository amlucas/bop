BIN = $(HOME)/bin # where to install
PROGS = bop2grid bop2line # what to install

CXX=g++
LINK=$(CXX)
CXXFLAGS = -std=c++11 -Wpedantic -Wall -O3

all: $(PROGS)

bop2grid.o: reader.h grid2vtk.h
bop2line.o: reader.h grid2vtk.h
grid2vtk.o: grid2vtk.h
reader.o: reader.h

bop2grid: bop2grid.o grid2vtk.o reader.o
bop2line: bop2line.o reader.o

%: %.o; $(LINK) $^ $(LDLIBS) -o $@

install: all
	mkdir -p $(BIN)
	cp $(PROGS) $(BIN)

clean:; rm -f $(PROGS) bop2grid.o bop2line.o grid2vtk.o reader.o

.PHONY: clean install all
