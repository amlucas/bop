# Create and install octave package

 # version
v = 0.1.0

# files in package
i = inst
O = COPYING DESCRIPTION $i/bop_version.m $i/bop_n.m \
    $i/bop_read.m $i/bop_grid.m $i/bop_igrid.m $i/bop_proj.m \
    $i/bop_write.m $i/bop_ply.m $i/efit.m $i/bop_join.m
n = bop-$v
p =     $n.tar.gz

install: pkg; octave --no-window-system --quiet --norc --eval "pkg install $p"

pkg: $p
$p: $O
	mkdir -p $n
	mkdir -p $n/inst
	for f in $O; do cp "$$f" $n/$$f; done
	tar zcf $@ $n && rm -r $n

clean:; rm -f $p

.PHONY: clean pkg install
