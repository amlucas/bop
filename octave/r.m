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


while 1
  b = nxt();
  if isempty(b); break; endif
  B1 = bop_ply("test_data/r.ply");
endwhile

# B = bop_ply("test_data/r.ply");
# [center, radii, evecs, v, chi2]  = efit([B.x', B.y', B.z']);
