#!/usr/bin/env octave-qf

1;
function r = eq(a, b); r = (strcmp(a, b) == 1); endfunction
function r = nxt()
  global iarg arg
  narg = numel(arg);
  if iarg <= narg; r = arg{iarg++}; else r = []; endif
endfunction

function r = nxt_num()
  r = nxt();
  r = str2num(r);
endfunction

function argp()
  global iarg arg
  iarg = 1;
  arg = argv();
  if eq(arg{1}, "-i"); arg = arg_default(); endif
endfunction

function a = arg_default()
  a = {"/home/lisergey/s/sh_12.0/ply/rbcs-01240.ply", "/home/lisergey/s/sh_12.0/ply/rbcs-01230.ply"};
endfunction

global e_c e_m
argp()

system("make >/dev/null");
pkg load bop
e_c = 0;

X = 1; Y = 2; Z = 3;
B = struct();
while !isempty(b = nxt())
  B0 = bop_ply(b);
  if e_c != 0; disp(e_m); exit; endif
  B =  bop_join(B0, B);
endwhile

[center, radii, R, v, chi2]  = efit([B.x', B.y', B.z']);

ax = radii(X); ay = radii(Y); az = radii(Z);

x = B.x; y = B.y; z = B.z;
vx = B.vx; vy = B.vy; vz = B.vz;

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
ab2 = ab^2;
ab4 = ab^4;
f = (ab*(ab2*vxz-vzx))/(ab4*zz+xx);

printf("%g\n", f);

vx0 = f *     ab  * z;
vz0 = f * (-1/ab) * x;

E  = (vx0 - vx).^2 + (vz0 - vz).^2;
E0 = vx.^2 + vz.^2;

R1 = E
sum(E/numel(E))
