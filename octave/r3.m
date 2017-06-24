#!/usr/bin/env octave-qf

pkg load bop
global e_c e_m; e_c = 0;
X = 1; Y = 2; Z = 3;

function B = read()
  global e_c e_m # error code and message
  pop = @bop_pop; ply = @bop_read_ply; join = @bop_join;
  B = struct();
  while !isempty(b = pop())
    B0 = ply(b);
    if e_c != 0; error(e_m); endif
    B =  join(B0, B);
  endwhile
endfunction

function [f, e, vx0, vz0] = sk_fit(x, z, vx, vz, q)
  # q: ax/az; e: sq. error (vector); v[xz]0: prediction
  vxz = sum(vx.*z); vzx = sum(vz.*x);
  xx  = sum(x.*x);   zz = sum(z.*z);
  q2 = q^2; q4 = q^4;
  f = (q*(q2*vxz-vzx))/(q4*zz+xx);

  vx0 = f *     q  * z;
  vz0 = f * (-1/q) * x;
  e  = (vx0 - vx).^2 + (vz0 - vz).^2;
endfunction

[S, F] = bop_read_off("test_data/sph.162.off");

## fake command line arguments for interactive session
bop_iset(glob("~/s/sh_3.0/ply/rbcs-01*.ply"))

B = read();
[center, a, R, v, chi2]  = efit([B.x', B.y', B.z']);
B = bop_rot_mat(B, R);
S = bop_scale(S, a);
###S = bop_rot_mat(S, R);

ker    = @bop_cubic;
cutoff = 1.0;
l  = {"vx", "vy", "vz"};
S = bop_p2p_accum(S, B, l, ker, cutoff);
S = bop_p2p_norm (S, l);

[f, e, vx0, vz0] = sk_fit(S.x, S.z, S.vx, S.vz, a(X)/a(Z));

bop_write_tri(S, F, "b.vtk");
	
printf("%g %g\n", f, a(X)/a(Z));
