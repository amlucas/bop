# Create and install octave package

 # version
v = 0.1.0

# files in package
i = inst
O = COPYING DESCRIPTION \
        $i/bop_eq.m $i/bop_igrid.m $i/bop_iset.m $i/bop_join.m $i/bop_n.m $i/bop_read_ply.m  \
        $i/bop_pop.m $i/bop_proj.m $i/bop_push.m $i/bop_read.m $i/bop_set.m $i/bop_version.m \
        $i/bop_write_grid.m $i/bop_write_vtk.m $i/bop_write_tri.m $i/bop_write.m $i/efit.m   \
        $i/bop_read_off.m $i/bop_p2p_accum.m $i/bop_p2p_norm.m $i/bop_cubic.m

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
