## reads ply file : output a structure B.x, B.y, ...
function B = bop_ply(fn)
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
