#!/usr/bin/env octave-qf

system("make > /dev/null");
pkg load bop
global e_m e_c

B.x = [1 2 3];
B.y = [1 2 3];
B.z = [1 2 3];
B.vx = [1000, 2000, 3000];

bop_write_vtk(B, "b.vtk");
