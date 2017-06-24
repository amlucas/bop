#!/usr/bin/env octave-qf

system("make > /dev/null");
pkg load bop
global e_m e_c

[B, F] = bop_read_ply("test_data/r.ply");

a = bop_area(B, F)

