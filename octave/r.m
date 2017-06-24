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

# interactive?
function r = iact()
  eq = @bop_eq;
  r = numel(a = argv()) >= 1 && eq(a{1}, "-i");
endfunction

if iact(); bop_set(glob("~/s/sh_3.0/ply/rbcs-01*.ply")); endif

[x, y, z, vx, vy, vz] = read();
[center, radii, R, v, chi2]  = efit([x', y', z']);
ax = radii(X); ay = radii(Y); az = radii(Z);

n = numel(x);
for i=1:n
  v = [vx(i) vy(i) vz(i)];
  v = v * R;
  vx(i) = v(X); vy(i) = v(Y); vz(i) = v(Z);
endfor

for i=1:n
  r = [x(i) y(i) z(i)];
  r = r * R;
  x(i) = r(X); y(i) = r(Y); z(i) = r(Z);
endfor
clear r

a = radii(X); b = radii(Z); ab = a/b;
vxz = sum(vx.*z); vzx = sum(vz.*x);
xx = sum(x.*x);    zz = sum(z.*z);
ab2 = ab^2; ab4 = ab^4;
f = (ab*(ab2*vxz-vzx))/(ab4*zz+xx);

printf("%g\n", f);

vx0 = f *     ab  * z;
vz0 = f * (-1/ab) * x;

E  = (vx0 - vx).^2 + (vz0 - vz).^2;
E0 = vx.^2 + vz.^2;
