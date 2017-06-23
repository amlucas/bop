#!/usr/bin/env octave-qf

system("make >/dev/null");
pkg load bop

n = [5 10 2];
s = [0.1 0.1 0.1];
o = [0 0 0];
X = 1; Y = 2; Z = 3;

l = {"vx", "vy", "n"};
G = bop_igrid(l, n, o, s);

bop_grid(G.D, G.n, G.o, G.s, "o.vtk");
