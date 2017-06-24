#!/usr/bin/env octave-qf

system("make > /dev/null");
pkg load bop
global e_m e_c

[Bs, Fs] = bop_read_off("test_data/sph.162.off");
B        = bop_read("test_data/1000.bop");

bop_write_tri(Bs, Fs, "s.vtk");
bop_write_vtk( B,     "b.vtk");

l  = {"vx", "vy", "vz"};
Bs = bop_p2p_accum(Bs, B, l); # accumulate
Bs = bop_p2p_norm (Bs, l);    # normalize by "density"

e_m
