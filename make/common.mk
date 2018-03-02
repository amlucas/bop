# where to install tools
INST_BIN = $(HOME)/bin
# where to install library
INST_LIB = $(HOME)/prefix/bop

CXXFLAGS ?= -std=c++11 -Wpedantic -Wall -O3
CXX      ?= g++
MPI_CXX  ?= mpic++
MPICXXFLAGS ?= $(CXXFLAGS)
