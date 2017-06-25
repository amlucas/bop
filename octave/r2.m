#!/usr/bin/env octave-qf

system("make > /dev/null");
pkg load bop
global e_m e_c

[B, F] = bop_read_ply("~/s/sh_12.0/ply/rbcs-01220.ply");
## [B, F] = bop_read_off("test_data/sph.162.off");

[a, A] = bop_area(B, F);
F.A = A;

bop_write_tri(B, F, "t.vtk");
e_m
