#!/usr/bin/env octave-qf

system("make > /dev/null");
pkg load bop
global e_m e_c

[Bs, Fs] = bop_read_off("test_data/sph.162.off");
B        = bop_read("test_data/1000.bop");

bop_write_tri(Bs, Fs, "s.vtk");
bop_write_vtk( B,     "b.vtk");

ker    = @bop_cubic;
cutoff = 1.0;

l  = {"vx", "vy", "vz"};

Bs = bop_p2p_accum(Bs, B, l, ker, cutoff); # accumulate
Bs = bop_p2p_norm (Bs, l);    # normalize by "density"

# Bs.vx

bop_write_tri(Bs, Fs, "t.vtk");
e_m
