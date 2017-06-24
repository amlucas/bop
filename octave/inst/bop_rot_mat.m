# rotate using matrix R
function B = bop_rot_mat(B, R)
  global e_c e_m
  if !isstruct(B)       ; e_m = "not a structure"; e_c = 1; return; endif
  if !(size(R) == [3 3]); e_m = "not 3x3 matrix"; e_c = 1;  return; endif

  if  isfield(B, "x");  B = rot_r(B, R); endif
  if isfield(B, "vx");  B = rot_v(B, R); endif
endfunction

function B = rot_r(B, R)
  [B.x,  B.y,  B.z] = rot(B.x,  B.y,  B.z, R);
endfunction

function B = rot_v(B, R)
  [B.vx, B.vy, B.vz] = rot(B.vx, B.vy, B.vz, R);
endfunction

function [x, y, z] = rot(x, y, z, R)
  X = 1; Y = 2; Z = 3;
  n = numel(x);
  for i=1:n
    r = [x(i) y(i) z(i)];
    r = r * R;
    x(i) = r(X); y(i) = r(Y); z(i) = r(Z);
  endfor
endfunction
