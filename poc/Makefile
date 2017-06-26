BIN = $(HOME)/bin # where to install
# what to install
P = octave-qf bop2grid bop2line h5bop h5vtk \
	r.h5bop r.h5vtk r.ply2vtk bop2g \
	bop.avel bop.avel.acc bop.avel.ks bop.area

CXX=g++
LINK=$(CXX)
CXXFLAGS = -std=c++11 -Wpedantic -Wall -O3

all: $(P) install

bop2grid.o: reader.h grid2vtk.h
bop2line.o: reader.h grid2vtk.h
grid2vtk.o: grid2vtk.h
reader.o: reader.h

bop2grid: bop2grid.o grid2vtk.o reader.o
bop2line: bop2line.o reader.o

%: %.o; $(LINK) $^ $(LDLIBS) -o $@

install:; mkdir -p $(BIN) && cp $P $(BIN)

clean:; rm -f $P bop2grid.o bop2line.o grid2vtk.o reader.o

.PHONY: clean install all
