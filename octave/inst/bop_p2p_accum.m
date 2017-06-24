# [p]articles [to] [p]articles interpolation (see also bop_p2p_norm.m)
# l: list of fields; w : kernel function; c : cutoff
function A = bop_p2p_accum(A, B, l, w, c)
  global e_c e_m
  if !isstruct(A) ; e_m = "not a structure"; e_c = 1; return; endif
  if !isfield(A, "x") ; e_m = "no x"; e_c = 1; return; endif
  if !isfield(A, "y") ; e_m = "no y"; e_c = 1; return; endif
  if !isfield(A, "z") ; e_m = "no z"; e_c = 1; return; endif

  if !isstruct(B) ; e_m = "not a structure"; e_c = 1; return; endif
  if !isfield(B, "x") ; e_m = "no x"; e_c = 1; return; endif
  if !isfield(B, "y") ; e_m = "no y"; e_c = 1; return; endif
  if !isfield(B, "z") ; e_m = "no z"; e_c = 1; return; endif

  if !iscell(l); e_m = "not a cell list"; e_c = 1; return; endif

  nv = numel(l);
  for i=1:nv; i = l{i};
      if !isfield(B, i); e_m = sprintf("%s in not in B", i); e_c = 1; return; endif
  endfor
  A = ini(A, l);

  ker.w = w; ker.c = c; # pack kernel
  A = accum(A, B, l, ker);
endfunction

function A = ini(A, l) # init fileds in `A'
  nv = numel(l); sz = size(A.x);
  for i=1:nv; i = l{i};
      if !isfield(A, i) A.(i) = zeros(sz); endif
  endfor
  # density and counter
  if !isfield(A, "den"); A.den = zeros(sz); endif
  if !isfield(A, "cnt"); A.cnt = zeros(sz); endif
endfunction

function A = accum(A, B, l, ker)
  n = numel(A.x); nv = numel(l);
  for i = 1:n
    r = [A.x(i), A.y(i), A.z(i)];
    A = accum0(A, B, l, nv, ker, r, i);
  endfor
endfunction

function A = accum0(A, B, l, nv, ker, r, i)
  ## f: field
  [idx, w] = interact(B, r, ker);
  A.den (i) += sum(w);
  A.cnt (i) += sum(idx);
  for f=1:nv; fn = l{f}; f = B.(fn); f = f(idx);
      A.(fn)(i) += sum(f .* w);
  endfor
endfunction

function [i, w] = interact(B, r, ker)
  X = 1; Y = 2; Z = 3;
  x = B.x; y = B.y; z = B.z;
  dx = x - r(X); dy = y - r(Y); dz = z - r(Z);
  r  = dx.^2 + dy.^2 + dz.^2;
  r  = sqrt(r);
  i  = r < ker.c;
  w  = ker.w(r(i));
endfunction

