# push argument from command line argument list
function bop_push()
  global arg iarg
  if iscell(arg) && iarg > 1; iarg--; endif
endfunction
