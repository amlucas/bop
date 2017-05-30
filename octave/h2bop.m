#!/usr/bin/env octave-qf

###### To vtk
# ./h2bop.m out /tmp/run_122/h5/s.h5part
# (cd out; for f in *.bop; do b=`basename $f .bop`; bop2vtk $b.vtk $f; done)

d = argv(){1}; # output dir
f = argv(){2}; # h5part file
fmt = "%04d";

function mkdir(d); c = sprintf("mkdir -p -- '%s'", d); system(c); endfunction

function ffi = fsort(ffi) % sort fieldnames
  nf = numel(ffi);
  fft = cell(nf, 1); idx = zeros(nf, 1);
  for i=1:nf; fi = ffi{i};
      t = sscanf(fi, "Step_%d");
      fft{i} = fi; idx(i) = t;
  endfor
  [~, idx] = sort(idx);
  ffi = {fft{idx}};
endfunction

DD = load(f);
ffi = fieldnames(DD);
ffi = fsort(ffi);

mkdir(d);
for i=1:numel(ffi); D = DD.(ffi{i});
    n = numel(D.x);
    l = "x y z vx vy vz type";
    D = [D.x; D.y; D.z; D.u; D.v; D.w; single(D.type)];

    b = sprintf(fmt, i);
    bop  = sprintf("%s/%s.bop", d, b);
    val0 = sprintf(   "%s.values", b);
    val  = sprintf("%s/%s.values", d, b);

    f = fopen(bop, "w");
    fprintf(f, "%ld\n", n);
    fprintf(f, "DATA_FILE: %s\n", val0);
    fprintf(f, "DATA_FORMAT: float\n");
    fprintf(f, "VARIABLES: %s\n", l);
    fclose(f);

    f = fopen(val, "w");
    fwrite(f, D, "single");
    fclose(f);
endfor