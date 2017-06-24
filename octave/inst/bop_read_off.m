## reads ply file : output a structure B.[xyz] (points) and F.f[123]
## (faces)
function [B, F] = bop_read_off(fn)
  global e_c e_m  # error code and message
  F = B = -1;
  f = fopen(fn);
  if f == -1
    e_c = 1; e_m = sprintf("cannot open file: %s", fn);
    return
  endif
  [n, nt] = header(f);
  if  e_c == 1; return; endif

  B = vert(f, n);
  F = tri(f,  nt);
  fclose(f);
endfunction

function [n, nt] = header(f)
  global e_c
  n = nt = -1;

  s = fscn(f, "%s"){1};
  if e_c == 1; e_m = "error reading off file"; return; endif
  if !eq(s, "OFF"); e_c = 1; e_m = "not off file"; return; endif

  ## [n] [tri]angles, number of edges (unused)
  [n, nt, ne] = fscn(f, "%d %d %d");
  n = dbl(n); nt = dbl(nt); ne = dbl(ne);
endfunction

function B = vert(f, n)
  dim = 3; # x, y, z
  D = dlmread(f, ' ', [0, 0, n - 1, dim - 1]);
  i = 1;
  B.x  = D(:, i++); B.y  = D(:, i++); B.z  = D(:, i++);
endfunction

function F = tri(f,  n)
  np = 4; # one line: 3 f1 f2 f3
  D = dlmread(f, ' ', [0, 0, n - 1, np - 1]);
  if !all(D(:, 1) == 3); F = -1; e_c = 1; e_m = "not triangles"; return; endif
  i = 2;
  F.f1 = D(:, i++); F.f2 = D(:, i++); F.f3 = D(:, i++);
endfunction

function varargout = fscn(f, fmt) # simpler fscanf
  global e_c
  l = fgets(f);
  if l == -1; e_c = 1; [varargout{1:nargout}] = -1; return; endif
  [varargout{1:nargout}] = strread(l, fmt);
endfunction

function e = dbl(e); e = double(e); endfunction
