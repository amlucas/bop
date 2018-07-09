M=../make
BOP=../src

include $M/usr.mk
include $M/common.mk
include $M/rules.mk

PYBIND_INC=$(PYBIND_PREFIX)/include
PYCONF=python3-config

LIBS      = -lbop -lbop_serial
LDFLAGS   = -L$(BOP)
CXXFLAGS += -I$(BOP)
CXXFLAGS += -I$(PYBIND_INC)

CXXFLAGS += `$(PYCONF) --includes`
LIBS     += `$(PYCONF) --ldflags`

CXXFLAGS += -fvisibility=hidden

PYEXT  = $(shell $(PYCONF) --extension-suffix)
TARGET = pybop$(PYEXT)

all: $(TARGET)

pybop.o: pybop.cpp pybop.h
binding.o: binding.cpp pybop.h

$(TARGET): binding.o pybop.o
	$(CXX) $(CXXFLAGS) -shared $^ -o $@ $(LDFLAGS) $(LIBS)

clean:; rm -f *.o $(TARGET)

test: $(TARGET)
	atest `find test -name main`

.PHONY: test clean test
