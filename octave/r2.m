#!/usr/bin/env octave-qf

system("make > /dev/null");
pkg load bop
global e_m e_c

[B, F] = bop_read_off("test_data/sph.162.off");
bop_write_tri(B, F, "t.vtk");
