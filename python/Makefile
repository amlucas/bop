M=../make
BOP=../src

include $M/usr.mk
include $M/common.mk
include $M/rules.mk

LIBS      = -lbop -lbop_serial
LDFLAGS   = -L$(BOP)
INCFLAGS  = -I$(BOP)


all: libbop.so

libbop.so: bop.o
	$(CXX) -shared -Wl,-install_name,$@ -o $@ $^ $(LDFLAGS) $(LIBS)

bop.o: bop.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c -fPIC $< -o $@ 

bop2vtk.o: bop2vtk.cpp check.h
bop2txt.o: bop2txt.cpp check.h

install:
	mkdir -p $(INST_BIN)
	cp $(PROGS) $(INST_BIN)

clean:
	rm -f *.o *.so

.PHONY: test clean install
