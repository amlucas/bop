## reads ply file : output a structure B.x, B.y, ...
function B = bop_ply(fn)
  global e_c e_m  # error code and message
  f = fopen(fn);
  if f == -1
    e_c = 1; e_m = sprintf("cannot open file: %s", fn);
    return
  endif

  B = read(f);
  if  e_c == 1; return; endif

  fclose(f);
endfunction

function B = read(f)
  nv = 6; # xyzuvw
  global e_c
  B = -1;

  s = fscn(f, "%s"){1};
  if e_c == 1; e_m = "error reading ply file"; return; endif
  if !eq(s, "ply"); e_c == 1; e_m = "not a ply file"; return; endif

  skip(f);
  n = fscn(f, "%s"){3}; n = str2num(n);
  skipn(f, 6 + 1 + 1 + 1); # xyzuvw, ...

  skip = 0; arch = "ieee-le";
  D = fread(f, n * nv, "single", skip, arch);
  D = reshape(D, nv, n);

  B = struct();
  i = 1;
  B.x  = D(i++, :); B.y  = D(i++, :); B.z  = D(i++, :);
  B.vx = D(i++, :); B.vy = D(i++, :); B.vz = D(i++, :);
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
