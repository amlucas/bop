#!/usr/bin/env octave-qf

1;
function r = nxt()
  global iarg
  narg = numel(argv());
  if iarg <= narg; r = argv(){iarg++}; else r = []; endif
endfunction

function r = nxt_num()
  r = nxt();
  r = str2num(r);
endfunction

global e_c e_m iarg
iarg = 1;

system("make >/dev/null");
e_c = 0;
pkg load bop
X = 1; Y = 2; Z = 3;

B = struct();

while 1
  b = nxt();
  if isempty(b); break; endif
  B0 = bop_ply(b);
  if e_c != 0; disp(e_m); exit; endif
  B =  bop_join(B0, B);
  break
endwhile

[center, radii, evecs, v, chi2]  = efit([B.x', B.y', B.z']);

x = B.x; y = B.y; z = B.z;
vx = B.vx; vy = B.vy; vz = B.vz;

a = radii(X); b = radii(Z); ab = a/b;
## svxy, svyx = sm(v[:, X]*r[:, Y]), sm(v[:, Y]*r[:, X])
## sxx,   syy = sm(r[:, X]*r[:, X]), sm(r[:, Y]*r[:, Y])
##     ab2 = ab**2
##     ab4 = ab**4
##     fr = (ab*(ab2*svxy-svyx))/(ab4*syy+sxx)

vxz = sum(vx.*z); vzx = sum(vz.*x);
xx = sum(x.*x);    zz = sum(z.*z);
ab2 = ab^2;
ab4 = ab^4;
fr = (ab*(ab2*vxz-vzx))/(ab4*zz+xx);

printf("%g\n", fr);
