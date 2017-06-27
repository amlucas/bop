#!/usr/bin/env octave-qf

1;
function usg()
  msg('usage: bop.avel.ks o.vkt [ply files]...\n')
  msg('  outputs average ellipsoid with KS-fit of the velocities\n')
  exit
endfunction
function msg(fmt, varargin); fprintf(stderr(), fmt, [varargin{:}]); endfunction

pkg load bop
global e_c e_m; e_c = 0;
X = 1; Y = 2; Z = 3;

if bop_eq(bop_pop(), "-h"); usg(); else bop_push(); endif

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

function [S, f] = ks(S, a)
  X = 1; Y = 2; Z = 3;
  x = S.x; z = S.z;
  vx = S.vx; vy = S.vy; vz = S.vz;
  [f, e, vx0, vz0] = bop_ks(x, z, vx, vz, a(X)/a(Z)); vy0 = zeros(size(vx0));

  A = max(abs(vx0));
  
  S.vx0 = vx0;
  S.vy0 = vy0;
  S.vz0 = vz0;
  
  S.dx  = (vx - vx0)/A;
  S.dy  = (vy - vy0)/A;
  S.dz  = (vz - vz0)/A;
endfunction


o = bop_pop(); # output file
[S, F] = bop_read_off("~/.bop/sph.162.off");
B = read();

[center, a, R, v, chi2]  = efit([B.x', B.y', B.z']);
S = bop_scale(S, a);
B = bop_rot_mat(B, R);

ker    = @bop_cubic;
cutoff = 1.0;
l  = {"vx", "vy", "vz"};
S = bop_p2p_accum(S, B, l, ker, cutoff); # accumulate
S = bop_p2p_norm (S, l);    # normalize by "den"

[S, f] = ks(S, a);
printf("%g %g %g %g\n", f, a(X), a(Z), a(Y));

bop_write_tri(S, F, o);

