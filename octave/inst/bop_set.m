# set default argument list for bop_pop
function bop_set(varargin)
  if nargin == 1 && iscell(fst = varargin{1})
    set0(fst)
  else
    set0(varargin)
  endif
endfunction

function set0(arg)
  global darg iarg
  darg = arg; iarg = 1;
endfunction
