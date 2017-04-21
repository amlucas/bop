BIN = $(HOME)/bin # where to install
PROGS = bop2vtk # what to install
OBJ = bop2vtk.o
CFLAGS = -std=c99 -pedantic -Wall -Os

all: $(PROGS)
%: %.o; $(CC) -o $@ $< ${LDFLAGS}

bop2vtk.o: endian.h bop.utils.h

install: all
	mkdir -p $(BIN)
	cp bop2vtk $(BIN)

clean:; rm -f $(PROGS) $(OBJ)
.PHONY: clean install
