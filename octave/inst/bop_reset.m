# reset command line arguments
function bop_reset()
  global darg arg iarg
  if iscell(darg); arg = darg;
  else           ; arg = argv();
  endif
  iarg = 1;
endfunction
