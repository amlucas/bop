# [p]articles [to] [p]articles interpolation (see also bop_p2p_norm.m)
function A = bop_p2p_accum(A, B, l)
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
endfunction
