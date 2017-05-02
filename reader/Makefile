BIN = $(HOME)/bin # where to install
CXXFLAGS = -std=c++11 -Wpedantic -Wall -O3
CXX=g++

SRC = $(wildcard *.cpp)
PROGS = $(SRC:.cpp=)


all: $(PROGS)
%: %.o; $(CXX) $(CXXFLAGS) -o $@ $< ${LDFLAGS}

bop2vtk.o: bop2vtk.cpp reader.decl.h reader.impl.h

install: all
	mkdir -p $(BIN)
	cp $(PROGS) $(BIN)

clean:; rm -f $(PROGS) *.o
.PHONY: clean install
