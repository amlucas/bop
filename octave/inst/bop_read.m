## reads bop file : output is a structure B.x, B.y, ...
function n = bop_read(fn)
  global e_c e_m  # error code and message
  f = fopen(fn);
  if f == -1
    e_c = 1; e_m = sprintf("cannot open file: %s", fn);
    return
  endif
  n = fscn(f, "%d");
  if e_c == 1
    e_m = sprintf("error reading `n': %s", fn);
    return
  endif
  fclose(f);
endfunction

function varargout = fscn(f, fmt) # simpler fscanf
  global e_c
  l = fgets(f);
  if l == -1; e_c = 1; [varargout{1:nargout}] = -1; return; endif
  [varargout{1:nargout}] = strread(l, fmt);
endfunction
