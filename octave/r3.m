#!/usr/bin/env octave-qf

system("make > /dev/null");
pkg load bop
global e_m e_c

rnd = @() unifrnd(-1, 1);

M = 100000; I = 0;
for i=1:M
  r = [rnd() rnd() rnd()];
  r = sum(r.^2);
  r = sqrt(r);
  w = bop_cubic(r);
  I += w;
endfor

[1/8, I/M]
