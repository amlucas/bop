M=../make
BOP=../src

include $M/usr.mk
include $M/common.mk
include $M/rules.mk

#TODO
PYBIND_DIR=$(HOME)/projects/pybind11/include
PYCONF=python3-config

LIBS      = -lbop -lbop_serial
LDFLAGS   = -L$(BOP)
CXXFLAGS += -I$(BOP)
CXXFLAGS += -I$(PYBIND_DIR)

CXXFLAGS += `$(PYCONF) --includes`
LIBS     += `$(PYCONF) --ldflags`

CXXFLAGS += -fvisibility=hidden

#PYEXT  = .so
PYEXT  = $(shell $(PYCONF) --extension-suffix)
TARGET = pybop$(PYEXT)

all: $(TARGET)

pybop.o: pybop.cpp pybop.h
binding.o: binding.cpp pybop.h

$(TARGET): binding.o pybop.o
	$(CXX) $(CXXFLAGS) -shared $^ -o $@ $(LDFLAGS) $(LIBS)

clean:; rm -f *.o $(TARGET)

.PHONY: test clean install
