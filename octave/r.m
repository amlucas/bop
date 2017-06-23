#!/usr/bin/env octave-qf

system("make >/dev/null");
pkg load bop

n =  [80     1    80];
s =  [1/20    4    1/20];
o =  [-2    -2    -2];
l = {"vx", "vy", "vz"};
G = bop_igrid(l, n, o, s);

l = glob("d/*.bop"); nl = numel(l);
for i=100:nl; i = l{i};
    B = bop_read(i);
    G = bop_proj(G, B);
endfor

bop_grid(G.D, G.n, G.o, G.s, "o.vtk");
