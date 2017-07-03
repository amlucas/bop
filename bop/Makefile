CXXFLAGS = -std=c++11 -Wpedantic -Wall -O3
CXX=g++

libbop.a: bop_reader.o
	ar rcs $@ $^

bop_reader.o: bop_reader.cpp bop_reader.h

%.o: %.cpp; $(CXX) $(CCXXFLAGS) $(OPT) $< -c -o $@

.PHONY: clean
clean:; rm -rf *.o *.a

