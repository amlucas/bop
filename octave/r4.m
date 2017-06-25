#!/usr/bin/env octave-qf

pkg load bop
global e_c e_m; e_c = 0;
X = 1; Y = 2; Z = 3;

function B = read(B, f)
  global e_c e_m # error code and message
  pop = @bop_pop; join = @bop_join; eq = @bop_eq;
  while !isempty(b = pop()) && !eq(b, "--")
    disp(b);
    B0 = f(b);
    if e_c != 0; error(e_m); endif
    B =  join(B0, B);
  endwhile
endfunction


[S, F] = bop_read_off("test_data/sph.162.off");
B = struct();
B = read(B, @bop_read_ply);
B = read(B, @bop_read);

[center, a, R, v, chi2]  = efit([B.x', B.y', B.z']);
B = bop_rot_mat(B, R);
bop_write_grid(B, "fi.all.vtk");
