CXXFLAGS = -std=c++11 -Wpedantic -Wall -O3
CXX=g++

libbop.a: bop_reader.o bop_writer.o bop_common.o
	ar rcs $@ $^

bop_reader.o: bop_reader.cpp bop_reader.h bop_macros.h bop_common.h
bop_writer.o: bop_writer.cpp bop_writer.h bop_macros.h bop_common.h
bop_common.o: bop_common.cpp bop_common.h bop_macros.h

%.o: %.cpp; $(CXX) $(CXXFLAGS) $(OPT) $< -c -o $@

.PHONY: clean
clean:; rm -rf *.o *.a

