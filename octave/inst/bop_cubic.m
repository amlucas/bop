# cubic kernel in 3D (see maxima/cubic3d.mac)
function w = bop_cubic(r); w = arrayfun(@ker, r); endfunction

function w = ker(r)
  if r > 1 || r < 0; w = 0; return; endif
  
  if   r < 1/2; w = r.^2*(6*r-6)+1;
  else          w = -2*(r-1).^3;
  endif
  A = 8/pi;
  w *= A;
endfunction
