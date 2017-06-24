#!/usr/bin/env octave-qf

system("make > /dev/null");
pkg load bop
global e_m e_c

[B, F] = bop_read_ply("test_data/r.ply");

nt = numel(F.f1);
F.id = 1:nt;

bop_write_tri(B, F, "t.vtk");
e_m
