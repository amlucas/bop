# project particles to grid
function G = bop_proj(G, B)
  global e_c e_m # error code and message
  X = 1; Y = 2; Z = 3;
  o = G.o; n = G.n; s = G.s;
  x = B.x; y = B.y; z = B.z;

  ix = (x  - o(X) + s(X)/2)/s(X) + 1;
  iy = (y  - o(Y) + s(Y)/2)/s(Y) + 1;
  iz = (z  - o(Z) + s(Z)/2)/s(Z) + 1;
  ix = floor(ix); iy = floor(iy); iz = floor(iz);

  ii =      (ix >=    1) & (iy >=    1) & (iz >=    1);
  ii = ii & (ix <= n(X)) & (iy <= n(Y)) & (iz <= n(Z));
  #ii = ii & (G.D.type == 2);
  fieldnames(B)
  
  ix = ix(ii); iy = iy(ii); iz = iz(ii);
  kk = sub2ind(n, ix, iy, iz);
  cnt = G.D.cnt;
  
  l = fieldnames(B); nv = numel(l);
  for f=1:nv; fn = l{f};
      if !isfield(G.D, fn); continue; endif
      f = B.(fn); f = f(ii);
      gd = G.D.(fn);
      i = 1; for k=kk; c = cnt(k); g = gd(k);
	g = (c*g + f(i++))/(c + 1);
	gd(k) = g;
      endfor
      G.D.(fn) = gd;
  endfor

  for k=kk; cnt(k) += 1; endfor; G.D.cnt = cnt;
endfunction
