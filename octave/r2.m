#!/usr/bin/env octave-qf

system("make > /dev/null");
pkg load bop
global e_m e_c

[B, F] = bop_read_off("test_data/sph.162.off");

p = pi/3;
cp = cos(p); sp = sin(p);

B = bop_scale(B, [1 2 3]);
B = bop_rot_mat(B, [cp sp  0;
		    sp -cp 0;
		    0   0  1]);

e_m

bop_write_tri(B, F, "t.vtk");
