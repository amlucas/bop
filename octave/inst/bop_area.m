## compute area of triangulated surface
## A: list of triangle areas; a : total area
function [a, A] = bop_area(B, F)
  n = numel(F.f1);
  A = zeros(1, n);
  for i=1:n
    f1 = F.f1(i); f2 = F.f2(i); f3 = F.f3(i);
    f1 = zo(f1); f2 = zo(f2); f3 = zo(f3);
    A(i) = area(B, f1, f2, f3);
  endfor
  a = sum(A);
endfunction

function a = area(B, f1, f2, f3)
  f0 = f1; f1 = f2; f2 = f3;  # change indexing
  a = area0(B, f0, f1, f2);
endfunction

function a = area0(B, f0, f1, f2)
  x = B.x; y = B.y; z = B.z;
  
  x0 = x(f0); y0 = y(f0); z0 = z(f0);
  x1 = x(f1); y1 = y(f1); z1 = z(f1);
  x2 = x(f2); y2 = y(f2); z2 = z(f2);

  ax = x1 - x0; ay = y1 - y0; az = z1 - z0;
  bx = x2 - x0; by = y2 - y0; bz = z2 - z0;
  a = sq(ay*bz-az*by) + sq(az*bx-ax*bz) + sq(ax*by-ay*bx);
  a = sqrt(a)/2;
endfunction

# [z]ero to [o]ne
function r = zo(e); r = e + 1 ; endfunction
function r = sq(e); r = e.^2  ; endfunction
