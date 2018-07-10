M=../make
BOP=../src

include $M/usr.mk
include $M/common.mk
include $M/rules.mk

PYBIND_INC=$(PYBIND_PREFIX)/include
PYCONF=python3-config

BOP_CXXFLAGS = -I$(BOP)
BOP_LDFLAGS  = -L$(BOP)
LIBS         = -lbop -lbop_serial

PYBIND_CXXFLAGS = -I$(PYBIND_INC)
PYBIND_LDFLAGS  =

PY_CXXFLAGS = `$(PYCONF) --includes`
PY_LDFLAGS  = `$(PYCONF) --ldflags`

INCFLAGS = $(BOP_CXXFLAGS)  \
	 $(PYBIND_CXXFLAGS) \
	 $(PY_CXXFLAGS)     \
	 -fvisibility=hidden

LDFLAGS = $(BOP_LDFLAGS)  \
	$(PYBIND_LDFLAGS) \
	$(PY_LDFLAGS)

PYEXT  = .so
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
.SUFFIXES:
