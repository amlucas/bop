#!/usr/bin/env octave-qf

pkg load bop
global e_c e_m; e_c = 0;
X = 1; Y = 2; Z = 3;

function [x, y, z, vx, vy, vz] = read()
  global e_c e_m # error code and message
  pop = @bop_pop; ply = @bop_ply; join = @bop_join;
  B = struct();
  while !isempty(b = pop())
    B0 = ply(b);
    if e_c != 0; error(e_m); endif
    B =  join(B0, B);
  endwhile
  x = B.x; y = B.y; z = B.z;
  vx = B.vx; vy = B.vy; vz = B.vz;
endfunction

function [x, y, z] = rot(x, y, z, R)
  X = 1; Y = 2; Z = 3;
  n = numel(x);
  for i=1:n
    r = [x(i) y(i) z(i)];
    r = r * R;
    x(i) = r(X); y(i) = r(Y); z(i) = r(Z);
  endfor
endfunction

function [f, E, vx0, vz0] = sk_fit(x, z, vx, vz, q)
  # q: ax/az; E: sq. error; v[xz]0: prediction
  vxz = sum(vx.*z); vzx = sum(vz.*x);
  xx  = sum(x.*x);   zz = sum(z.*z);
  q2 = q^2; q4 = q^4;
  f = (q*(q2*vxz-vzx))/(q4*zz+xx);

  vx0 = f *     q  * z;
  vz0 = f * (-1/q) * x;
  E  = (vx0 - vx).^2 + (vz0 - vz).^2;
endfunction

## fake command line arguments for interactive session
bop_iset(glob("~/s/sh_3.0/ply/rbcs-01*.ply"))

[x, y, z, vx, vy, vz] = read();
[center, a, R, v, chi2]  = efit([x', y', z']);
[ x,  y,  z] = rot( x,  y,  z, R);
[vx, vy, vz] = rot(vx, vy, vz, R);
[f, E] = sk_fit(x, z, vx, vz, a(X)/a(Z));
	
printf("%g\n", f);
