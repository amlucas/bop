# if session interactive set arguments for bop_pop
function bop_iset(arg)
  iact();
  if iact(); bop_set(arg); endif
endfunction

# [i]nter[act]ive?
function r = iact()
  eq = @bop_eq;
  n = numel(a = argv());
  for i=1:n; i = a{i};
      if eq(i, "-i") || eq(i, "--interactive"); r = true; return; endif
  endfor
  r = false;
endfunction
