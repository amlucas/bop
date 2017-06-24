# scale [xyz] and v[xyz] by `s[3] = [sx, sy, sz]
function B = bop_scale(B, s)
  X = 1; Y = 2; Z = 3;

  if isfield(B, "x"); B.x *= s(X); endif
  if isfield(B, "y"); B.y *= s(Y); endif
  if isfield(B, "z"); B.z *= s(Z); endif

  if isfield(B, "vx"); B.vx *= s(X); endif
  if isfield(B, "vy"); B.vy *= s(Y); endif
  if isfield(B, "vz"); B.vz *= s(Z); endif
endfunction
