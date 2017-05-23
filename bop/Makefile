CXXFLAGS = -std=c++11 -Wpedantic -Wall -O3
CXX=g++

libbop.a: reader.o
	ar rcs $@ $^

reader.o: reader.cpp reader.h

%.o: %.cpp; $(CXX) $(CCXXFLAGS) $(OPT) $< -c -o $@

.PHONY: clean
clean:; rm -rf *.o *.a

