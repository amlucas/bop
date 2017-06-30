Tools for BOP (brick of particles) files
========================================

Introduction
------------
A set of particles can be represented by 2 to 3 files: 
- `.bop` file, containing description of the data.
- `.values` file, containing the positions of the particles and optional floating point fields such as velocity
- (optional) `.int` file, containing integer values (e.g. global id, tag...)

`file.bop` format (ascii, `<N>` is the number of particles):

	 <N>
	 DATA_FILE: <file.values>
	 DATA_FORMAT: <float|double|ascii>
	 VARIABLES: <x> <y> <z> <vx> <vy> <vz> ...
     INT_FILE: <file.int>
     VARIABLES: <id> <tag> ...

(last 2 lines are optional)

`file.values` format:


	x[0] y[0] z[0] vx[0] vy[0] vz[0]
	...

	x[N-1] y[N-1] ...

`file.int` format:


	id[0] tag[0] ...
	...

	id[N-1] tag[N-1] ...

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
Uses `atest` framework (https://gitlab.ethz.ch/mavt-cse/atest)

```sh
atest bop2txt.cpp
atest bop2vtk.cpp
```
