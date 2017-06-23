# [i]nitilize [grid]
function G = bop_igrid(l, n, o, s)
  global e_c e_m # error code and message

  G = struct();
  if !iscell(l)
    e_m = "l is not cell of strigns";
    e_c = 1; G = -1;
    return
  endif
  G.n = n; G.o = o; G.s = s;
  if numel(n) != 3; e_m = "wrong n"        ; e_c = 1; return; endif
  if numel(o) != 3; e_m = "wrong o"        ; e_c = 1; return; endif
  if numel(s) != 3; e_m = "wrong s"        ; e_c = 1; return; endif

  D = struct();
  nv = numel(l);
  for i=1:nv; i = l{i}; D.(i) = zeros(n); endfor
  D.nden = D.cnt = zeros(n); # number density and counter
  
  G.D = D;
endfunction
