# [p]articles [to] [p]articles interpolation : normalize data
# collected by bop_p2p_accum by A.den and removes A.den
function A = bop_p2p_norm(A, l)
  global e_c e_m
  if !isstruct(A) ; e_m = "not a structure"; e_c = 1; return; endif
  if !isfield(A, "den") ; e_m = "no den"; e_c = 1; return; endif
  if !iscell(l); e_m = "not a cell list"; e_c = 1; return; endif

  nv = numel(l);
  for i=1:nv; i = l{i};
      if !isfield(A, i); e_m = sprintf("%s in not in A", i); e_c = 1; return; endif
  endfor

  A = norm(A, l);
  A = clean(A);
endfunction

function A = norm(A, l)
  i = A.den > 0;
  den = A.den(i);
  A = norm0(A, l, den, i);
endfunction

function A = norm0(A, l, den, i)
  nv = numel(l);
  for f=1:nv; f = l{f};
      A.(f)(i) ./= den;
  endfor
endfunction

function A = clean(A); A = rmfield(A, "den"); endfunction
