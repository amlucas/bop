## Keller-Skalak fit of the velocities
## q: ax/az;  v[xz]0: prediction
## f: frequency, e: sq. error (vector)
function [f, e, vx0, vz0] = bop_ks(x, z, vx, vz, q)
  vxz = sum(vx.*z); vzx = sum(vz.*x);
  xx  = sum(x.*x);   zz = sum(z.*z);
  q2 = q^2; q4 = q^4;
  f = (q*(q2*vxz-vzx))/(q4*zz+xx);

  vx0 = f *     q  * z;
  vz0 = f * (-1/q) * x;
  e  = (vx0 - vx).^2 + (vz0 - vz).^2;
endfunction
