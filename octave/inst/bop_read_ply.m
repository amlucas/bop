## reads ply file : output a structure B.x, B.y, ...
function [B, F] = bop_read_ply(fn)
  global e_c e_m  # error code and message
  F = B = -1;
  f = fopen(fn);
  if f == -1
    e_c = 1; e_m = sprintf("cannot open file: %s", fn);
    return
  endif
  nv = 6; # xyzuvw
  [n, nt] = read_header(f, nv);
  if  e_c == 1; return; endif

  B = read_vert(f, n, nv);
  F = read_tri(f,  nt);
  fclose(f);
endfunction

function [n, nt] = read_header(f, nv)
  global e_c
  B = -1;

  s = fscn(f, "%s"){1};
  if e_c == 1; e_m = "error reading ply file"; return; endif
  if !eq(s, "ply"); e_c == 1; e_m = "not a ply file"; return; endif

  skip(f); # format ...
  n = fscn(f, "%s"){3}; n = str2num(n);
  skipn(f, nv); # xyzuvw

  # [n] [tri]angles
  nt = fscn(f, "%s"){3}; nt = str2num(nt);
  skipn(f, 2); # prop...
               # end_header
endfunction

function B = read_vert(f, n, nv)
  D = le_float(f, n * nv); # little endian
  D = reshape(D, nv, n);

  B = struct();
  i = 1;
  B.x  = D(i++, :); B.y  = D(i++, :); B.z  = D(i++, :);
  B.vx = D(i++, :); B.vy = D(i++, :); B.vz = D(i++, :);
endfunction

function F = read_tri(f,  nt)
  F = -1
  np = 4; # one line: 3 f1 f2 f3
  D = le_int(f, nt * np);
  D = reshape(D, np, nt);
  if !all(D(1, :) == 3); e_c = 1; e_m = "not triangles"; return; endif

  F = struct(); i = 2;
  F.f1 = D(i++, :); F.f2 = D(i++, :); F.f3 = D(i++, :);
endfunction

function skipn(f, n); while n-- > 0; skip(f); endwhile; endfunction
function skip(f); s = fscn(f, "%s"); endfunction

function varargout = fscn(f, fmt) # simpler fscanf
  global e_c
  l = fgets(f);
  if l == -1; e_c = 1; [varargout{1:nargout}] = -1; return; endif
  [varargout{1:nargout}] = strread(l, fmt);
endfunction

function r = eq(a, b); r = (strcmp(a, b) == 1); endfunction

function D = le_float(f, n)
  skip = 0; arch = "ieee-le";
  D = fread(f, n, "float", skip, arch);
endfunction

function D = le_int(f, n)
  skip = 0; arch = "ieee-le";
  D = fread(f, n, "int", skip, arch);
endfunction
