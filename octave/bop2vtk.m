% bop to vtk converter

fn  = '../test_data/0005000.bop';
fo  = 'o.vtk';

fd = fopen(fn);
n = fread(fd, 1, 'int32');
D = fread(fd, 'float32');
fclose(fd);
D = reshape(D, [], n);

p = @(fmt, varargin) fprintf(fd, [fmt, '\n'], varargin{:});
fd = fopen(fo, 'w');
p('# vtk DataFile Version 2.0');
p('Created with bop2vtk.m');
p('BINARY');
p('DATASET POLYDATA');
p('POINTS %ld %s', n, 'float32');
fwrite(fd, D(1:3, :), 'float32', 0, 'ieee-be');
fclose(fd);
