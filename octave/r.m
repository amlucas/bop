#!/usr/bin/env octave-qf

system("make >/dev/null");
pkg load bop

n =  [40     1    40];
s =  [1/10   2  1/10];
o =  [-2    -1    -2];
l = {"vx", "vy", "vz"};
G = bop_igrid(l, n, o, s);

B = bop_read("test_data/1000.bop");
G = bop_proj(G, B);

bop_grid(G.D, G.n, G.o, G.s, "o.vtk");
