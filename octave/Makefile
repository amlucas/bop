# Create and install octave package

 # version
v = 0.1.0

# config dir and files
c = $(HOME)/.bop
C = data/sph.162.off

# files in package
i = inst
O = COPYING DESCRIPTION \
        $i/bop_eq.m $i/bop_igrid.m $i/bop_iset.m $i/bop_join.m $i/bop_n.m $i/bop_read_ply.m  \
        $i/bop_pop.m $i/bop_proj.m $i/bop_push.m $i/bop_read.m $i/bop_set.m $i/bop_version.m \
        $i/bop_write_grid.m $i/bop_write_vtk.m $i/bop_write_tri.m $i/bop_write.m $i/efit.m   \
        $i/bop_read_off.m $i/bop_p2p_accum.m $i/bop_p2p_norm.m $i/bop_cubic.m $i/bop_scale.m \
        $i/bop_rot_mat.m $i/bop_reset.m $i/bop_ellipse_arc.m $i/bop_area.m $i/bop_shuffle_grid.m \
        $i/bop_ks.m

n = bop-$v
p =     $n.tar.gz

install: pkg oct conf
oct:; octave --no-window-system --quiet --norc --eval "pkg install $p"
conf:; mkdir -p $c; cp $C $c

pkg: $p
$p: $O
	mkdir -p $n
	mkdir -p $n/inst
	for f in $O; do cp "$$f" $n/$$f; done
	tar zcf $@ $n && rm -r $n

clean:; rm -f $p

.PHONY: clean pkg install oct conf
