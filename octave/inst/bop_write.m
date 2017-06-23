## writes bop file
function bop_write(B, bn)
  global e_c e_m  # error code and message
  b = fopen(bn, "w");
  if b == -1
    e_c = 1; e_m = sprintf("cannot write file: %s", bn);
    return
  endif
  
  [d, v0]  = fileparts(bn);
  ## v0: value file name, vn: value file path
  vn = fullfile(d, v0);
  v = fopen(vn, "w");
  if v == -1
    e_c = 1; e_m = sprintf("cannot write file: %s", vn);
  endif
  write(B, b, v, v0);
  if e_c == 1; e_m = "cannot write data"; return; endif

  fclose(v);
  fclose(b);
endfunction

function write(B, b, v, v0)
  global e_c
  l = fieldnames(B);
  nv = numel(l);
  if nv == 0; e_c = 1; return; endif

  f = fst(l);
  n = numel(B.(f));
  write_meta(n, l, b, v0)
  write_data(B, n, v);
endfunction

function write_meta(n, l, b, v0)
  fprintf(b, "%ld\n", n);
  fprintf(b, "DATA_FILE: %s\n", v0);
  fprintf(b, "DATA_FORMAT: float\n");
  fprintf(b, "VARIABLES:");
  for i=1:numel(l); i = l{i}; fprintf(b, " %s", i); endfor
endfunction

function write_data(B, n, v)
  l = fieldnames(B);
  nv = numel(l);

  D = zeros(nv, n);
  for i=1:numel(l); f = l{i}; D(i, :) = B.(f); endfor
  fwrite(v, D, "single");
endfunction

function f = fst(l); f = l{1}; endfunction
