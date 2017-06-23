#!/usr/bin/env octave-qf

pkg load bop

n = [5 10 2];
s = [0.1 0.1 0.1];
o = [0 0 0];

X = 1; Y = 2; Z = 3;

vx = zeros(n);
for ix=1:n(X); for iy = 1:n(Y); for iz = 1:n(Z)
  vx(ix,iy,iz) = ix;
  vy(ix,iy,iz) = ix + iy;
endfor; endfor; endfor

D = struct();
D.vx = vx;  D.vy = vy;
bop_grid(D, n, o, s, "r.out.vtk");
