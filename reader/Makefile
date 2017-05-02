BIN = $(HOME)/bin # where to install
CXXFLAGS = -std=c++11 -Wpedantic -Wall -O3
CXX=g++

PROGS = bop2vtk
OBJS  = reader.o

all: $(PROGS)
%: %.o $(OBJS); $(CXX) $(CXXFLAGS) -o $@ $< $(OBJS) ${LDFLAGS}

reader.o: reader.cpp reader.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

#bop2txt.o: bop2txt.cpp reader.decl.h reader.impl.h
bop2vtk.o: bop2vtk.cpp reader.h

install: all
	mkdir -p $(BIN)
	cp $(PROGS) $(BIN)

clean:; rm -f $(PROGS) *.o
.PHONY: clean install
