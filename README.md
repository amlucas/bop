Tools for BOP (brick of particles) files
========================================

Introduction
============
`file.bop` format (ascii, `<N>` is a number of particles):

     <N>
     DATA_FILE: <file.values>
     DATA_FORMAT: <float|double|ascii>
     VARIABLES: <x> <y> <z> <vx> <vy> <vz> <id> ...

`file.values` format:


    x[0] y[0] z[0] vx[0] vy[0] vz[0] id[0]
    ...

    x[N-1] y[N-1] ...
	

Installation
============

```sh
make && make install
```
