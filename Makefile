BIN = $(HOME)/bin # where to install
PROGS = bop2vtk bop2txt # what to install
OBJ = bop2vtk.o bop2txt
CFLAGS = -std=c99 -pedantic -Wall -Os

all: $(PROGS)
%: %.o; $(CC) -o $@ $< ${LDFLAGS}

bop2vtk.o: bop.utils.h endian.h
bop2txt.o: bop.utils.h

install: all
	mkdir -p $(BIN)
	cp $(PROGS) $(BIN)

clean:; rm -f $(PROGS) $(OBJ)
.PHONY: clean install
