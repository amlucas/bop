% read example

fn  = '../test_data/0008000.bop';

fd = fopen(fn);
n = fread(fd, 1, 'int64')
D = fread(fd, 'float32');
fclose(fd);

D = reshape(D, [], n);
xx  = D(1, :);  yy = D(2, :);  zz = D(3, :);
vvx = D(4, :); vvy = D(5, :); vvz = D(6, :);
