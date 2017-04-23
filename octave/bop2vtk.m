#!/usr/bin/env octave-qf

% usage: bop2vtk.m output.file input.file
err   = @(varargin) fprintf(stderr(), varargin{:});
usage = @() err('usage: bop2vtk.m output.file input.file');

if (numel(argv()) != 2); usage(); exit(1); end
fo = argv(){1}; % output
fi = argv(){2}; % input

fd = fopen(fi);
if fd == -1; err('bop2vtk.m: cannot open %s', fi); exit(1); end
n = fread(fd, 1, 'int64'); n = -10;
if n < 0 || n > 1e8; err('bop2vtk.m: wrong n: %ld', n); exit(1); end
D = fread(fd, 'float32');
fclose(fd);
D = reshape(D, [], n);

p = @(fmt, varargin) fprintf(fd, [fmt, '\n'], varargin{:});
fd = fopen(fo, 'w');
if fd == -1; err('bop2vtk.m: cannot open %s', fo); exit(1); end
p('# vtk DataFile Version 2.0');
p('Created with bop2vtk.m');
p('BINARY');
p('DATASET POLYDATA');
p('POINTS %ld %s', n, 'float32');
fwrite(fd, D(1:3, :), 'float32', 0, 'ieee-be');
fclose(fd);
