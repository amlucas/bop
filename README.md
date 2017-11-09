Tools for BOP (brick of particles) files
========================================

Introduction
------------
`file.bop` format (ascii, `<N>` is the number of particles):

	 <N>
	 DATA_FILE: <file.values>
	 DATA_FORMAT: <float|ascii|double|int|iascii>
	 VARIABLES: <x> <y> <z> <vx> <vy> <vz> <id> ...

`file.values` format:


	x[0] y[0] z[0] vx[0] vy[0] vz[0] id[0]
	...

	x[N-1] y[N-1] ...

The `ascii` format is assumed to be single precision floating points data.  
Th `iascii` format is the ascii version of integer data.

Installation
------------

The following will install the binaries into `${HOME}/bin`. This folder needs to be in the `PATH` variable.
Furthermore, it installs the headers and library into `${HOME}/prefix/bop/include` and `${HOME}/prefix/bop/lib`, respectively.
```sh
make && make install
```

Usage
-----

Convert bop files `in1.bop`, `in2.bop`, ..., `inN.bop` into a single vtk file `out.vtk`:
```sh
bop2vtk out.vtk in1.bop in2.bop ... inN.bop
```

Dump ascii data from bop files `in1.bop`, `in2.bop`, ..., `inN.bop` into `out.txt`:
```sh
bop2txt in1.bop in2.bop ... inN.bop > out.txt
```

Testing
-------
Uses `atest` framework, see [gitlab page](https://gitlab.ethz.ch/mavt-cse/atest)

```sh
make test
```
