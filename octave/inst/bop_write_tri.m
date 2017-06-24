# write particles to vtk
function bop_write_tri(B, F, fn)
  global e_c e_m  # error code and message
  if !isstruct(B) ; e_m = "not a structure"; e_c = 1; return; endif
  if !isfield(B, "x") ; e_m = "no x"; e_c = 1; return; endif
  if !isfield(B, "y") ; e_m = "no y"; e_c = 1; return; endif
  if !isfield(B, "z") ; e_m = "no z"; e_c = 1; return; endif

  if !isstruct(F) ; e_m = "not a structure"; e_c = 1; return; endif
  if !isfield(F, "f1") ; e_m = "no f1"; e_c = 1; return; endif
  if !isfield(F, "f2") ; e_m = "no f2"; e_c = 1; return; endif
  if !isfield(F, "f3") ; e_m = "no f3"; e_c = 1; return; endif

  f = fopen(fn, "w");
  if f == -1
    e_c = 1; e_m = sprintf("cannot write vtk file: %s", fn);
    return
  endif
  write(B, F, f);
  fclose(f);
endfunction

function write(B, F, f)
  type = "float";
  rm = @rmfield;
  header(f);
  n = numel(B.x);
  topology_header(f, n, type);
  topology_data(f, B, type);
  cells(f, F);
  
  B = rm(B, "x"); B = rm(B, "y"); B = rm(B, "z");
  l = fieldnames(B); nv = numel(l);
  if nv > 0; vert_data_header(f, n); endif
  for i=1:nv; i=l{i}; vert_scalar(f, B.(i), i, type); endfor

  nt = numel(F.f1);

  F = rm(F, "f1"); F = rm(F, "f2"); F = rm(F, "f3");
  l = fieldnames(F); nv = numel(l);
  if nv > 0; cell_data_header(f, nt); endif
  for i=1:nv; i=l{i}; cell_scalar(f, F.(i), i, type); endfor
endfunction

function header(f)
  fprintf(f, "# vtk DataFile Version 2.0\n")
  fprintf(f, "created with bop_write_vtk\n")
  # fprintf(f, "ASCII\n")
  fprintf(f, "BINARY\n");
endfunction

function topology_header(f, n, type)
  fprintf(f, "DATASET POLYDATA\n");
  fprintf(f, "POINTS %ld %s\n", n, type);
endfunction

function topology_data(f, B, type)
  D = horzcat(B.x(:), B.y(:), B.z(:));
  D = D';
  be_write(f, D, type);
endfunction

function vert_data_header(f, n)
  fprintf(f, "POINT_DATA %ld\n", n);
endfunction

function vert_scalar(f, D, name, type)
  fprintf(f, "SCALARS %s %s\n", name, type);
  fprintf(f, "LOOKUP_TABLE default\n");
  be_write(f, D, type);
endfunction

function cell_scalar(f, D, name, type)
  fprintf(f, "SCALARS %s %s\n", name, type);
  fprintf(f, "LOOKUP_TABLE default\n");
  be_write(f, D, type);
endfunction

function cell_data_header(f, n); fprintf(f, "CELL_DATA %d\n", n); endfunction

function cells(f, F)
  np = 3;
  nt = numel(F.f1);
  sz = (1 + np) * nt;
  fprintf(f, "POLYGONS %d %d\n", nt, sz);

  npt = np * ones(1, nt);
  D = horzcat(npt(:), F.f1(:), F.f2(:), F.f3(:));
  D = D';
  be_write(f, D, "int");
endfunction

function be_write(f, D, type) # big-endian write
  skip = 0; arch = "ieee-be";
  fwrite(f, D, type, skip, arch);
  fprintf(f, "\n");
endfunction
