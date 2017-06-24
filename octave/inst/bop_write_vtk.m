# write particles to vtk
function bop_write_vtk(B, fn)
  global e_c e_m  # error code and message
  if !isstruct(B) ; e_m = "not a structure"; e_c = 1; return; endif
  if !isfield(B, "x") ; e_m = "no x"; e_c = 1; return; endif
  if !isfield(B, "y") ; e_m = "no y"; e_c = 1; return; endif
  if !isfield(B, "z") ; e_m = "no z"; e_c = 1; return; endif
  
  f = fopen(fn, "w");
  if f == -1
    e_c = 1; e_m = sprintf("cannot write vtk file: %s", fn);
    return
  endif
  write(B, f);
  fclose(f);
endfunction

function write(B, f)
  type = "float";
  rm = @rmfield;
  header(f);
  n = numel(B.x);
  topology_header(f, n, type);
  topology_data(f, B, type);
  
  B = rm(B, "x"); B = rm(B, "y"); B = rm(B, "z");

  l = fieldnames(B); nv = numel(l);
  if nv > 0; data_header(f, n); endif
  for i=1:nv; i=l{i}; scalar(f, B.(i), i, type); endfor
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

function data_header(f, n)
  fprintf(f, "POINT_DATA %ld\n", n);
endfunction

function scalar(f, D, name, type)
  fprintf(f, "SCALARS %s %s\n", name, type);
  fprintf(f, "LOOKUP_TABLE default\n");
  be_write(f, D, type);
endfunction

function be_write(f, D, type) # big-endian write
  skip = 0; arch = "ieee-be";
  fwrite(f, D, type, skip, arch);
  fprintf(f, "\n");
endfunction
