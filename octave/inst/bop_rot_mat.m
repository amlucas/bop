# rotate using matrix R
function B = bop_rot_mat(B, R)
  if !isstruct(B)     ; e_m = "not a structure"; e_c = 1; return; endif
  if size(R) == [3 3] ; e_m = "not 3x3 matrix"; e_c = 1; return; endif

  x  =  B.x;  y =  B.y;  z  = B.z;
  vx = B.vx; vy = B.vy; vz = B.vz;
  
  [ x,  y,  z] = rot( x,  y,  z, R);
  [vx, vy, vz] = rot(vx, vy, vz, R);

  B.x  =  x;  B.y  =  y;  B.z  =  z;
  B.vx = vx;  B.vy = vy;  B.vz = vz;
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
