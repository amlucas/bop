## reads bop file : output is a structure B.x, B.y, ...
function B = bop_read(fn)
  global e_c e_m  # error code and message
  f = fopen(fn);
  if f == -1
    e_c = 1; e_m = sprintf("cannot open file: %s", fn);
    return
  endif
  B = bop_read0(f);
  if e_c == 1
    e_m = sprintf("error reading file: %s", fn);
    return
  endif
  fclose(f);
endfunction

function B = bop_read0(f)
  global e_c
  ## n: number of part, v: values file, t: type, l: list of variables
  n = fscn(f, "%d");
  if e_c == 1; B = -1; return; endif

  v = fscn(f, "DATA_FILE: %s"){1};
  if e_c == 1; B = -1; return; endif

  t = fscn(f, "DATA_FORMAT: %s"){1};
  if e_c == 1; B = -1; return; endif

  l = fscn(f, "%s");
  if e_c == 1; B = -1; return; endif
  l = tail(l);

endfunction

function l = tail(l); l = {l{2:end}}; endfunction

function varargout = fscn(f, fmt) # simpler fscanf
  global e_c
  l = fgets(f);
  if l == -1; e_c = 1; [varargout{1:nargout}] = -1; return; endif
  [varargout{1:nargout}] = strread(l, fmt);
endfunction
