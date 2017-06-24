## joint bop files
function B0 = bop_join(B0, B1)
  global e_c e_m  # error code and message
  if !isstruct(B0); e_c = 1; e_m = "argument is not struct"; return; endif
  if !isstruct(B1); e_c = 1; e_m = "argument is not struct"; return; endif

  l = fieldnames(B0); nv = numel(l);
  for i=1:nv; i = l{i};
      if !isfield(B1, i); continue; endif
      f0 = B0.(i); f1 = B1.(i);
      f0 = horzcat(f0, f1);
      B0.(i) = f0;
  endfor
endfunction
