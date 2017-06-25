## shuffle dimensions in grid
## if code == yxz: old y becomes x, old x becomes y.
function G = bop_shuffle_grid(G, c)
  X = 1; Y = 2; Z = 3;
  c = decode(c);
  G  = header(G, c);
  G.D =  data(G.D, c);
  if isfield(G.D, "vx"); G.D =  vel(G.D, c); endif
endfunction

function D = vel(D, c)
  X = 1; Y = 2; Z = 3;
  s = {D.vx, D.vy, D.vz};
  s = {s{c}};
  D.vx = s{X}; D.vy = s{Y}; D.vz = s{Z};
endfunction

function G = header(G, c)
  n = G.n; o = G.o; s = G.s;
  n = n(c); o = o(c); s = s(c);
  G.n = n; G.o = o; G.s = s;
endfunction

function D = data(D, c)
  l = fieldnames(D);
  nv = numel(l);
  for f=1:nv; f=l{f};
      D.(f) = permute(D.(f), c);
  endfor
endfunction

function ans = decode(c)
  ans = zeros(1, 3);
  for i=1:3; ans(i) = c(i) - 'x' + 1; endfor
endfunction
