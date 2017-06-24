# pop argument from command line argument list
function a = bop_pop()
  global arg iarg
  if !iscell(arg); arg = argv(); iarg = 1; endif
  
  narg = numel(arg);
  if iarg <= narg; a = arg{iarg++}; else a = []; endif
endfunction
