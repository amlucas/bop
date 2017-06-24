# circumference of an ellipse
function c = bop_ellipse_arc(a, b, lo = 0, hi = 2*pi)
  arc = @(t) sqrt((a.*sin(t)).^2 + (b.*cos(t)).^2);
  c = quadgk(arc, lo, hi);
endfunction
