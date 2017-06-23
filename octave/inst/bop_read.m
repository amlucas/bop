## reads bop file : output is a structure B.x, B.y, ...
function B = bop_read(fn)
  global e_c e_m  # error code and message
  f = fopen(fn);
  if f == -1
    e_c = 1; e_m = sprintf("cannot open file: %s", fn);
    return
  endif
  
  d = fileparts(fn); # dirname
  B = read1(f, d);
  
  if     e_c == 1; e_m = sprintf("error reading file: %s", fn); return;
  elseif e_c == 2; return; endif
  
  fclose(f);
endfunction

function B = read1(f, d)
  global e_c e_m
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


  v = fullfile(d, v); # full path to values
  B = read0(v, n, l, t);
  if e_c == 1
    B = -1
    e_c = 2
    e_m = sprintf("error reading values: %s", v);
  endif

endfunction

function B = read0(v, n, l, t)
  global e_c
  f = fopen(v);

  if f == -1; B = -1; e_c = 1; return; endif
  D = fread(f, 'single');
  nv = numel(l); # number of variables
  if nv*n != numel(D); B = -1; e_c = 1; return; endif
  D = reshape(D, nv, n);
  B = struct();
  for i=1:nv; fi = l{i}; ## [fi]eld of structure
      B.(fi) = D(i, :);
  endfor
  fclose(f);
endfunction

function l = tail(l); l = {l{2:end}}; endfunction

function varargout = fscn(f, fmt) # simpler fscanf
  global e_c
  l = fgets(f);
  if l == -1; e_c = 1; [varargout{1:nargout}] = -1; return; endif
  [varargout{1:nargout}] = strread(l, fmt);
endfunction
