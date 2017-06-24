#!/usr/bin/env octave-qf

system("make > /dev/null");
pkg load bop
global e_m e_c

[Bs, Fs] = bop_read_off("test_data/sph.162.off");
B        = bop_read("test_data/1000.bop");

bop_write_tri(Bs, Fs, "s.vtk");
bop_write_vtk( B,     "b.vtk");

Bs = bop_p2p_accum(Bs, B, {"vx", "vy", "vz"}); # accumulate
Bs = bop_p2p_norm (Bs);                        # normalize by "density"

e_m
