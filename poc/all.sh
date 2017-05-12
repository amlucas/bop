#!/bin/bash

ggd () { awk '/gamma_dot/{print $3}' $d/conf.h ; }

gp () {
    gnuplot <<!
    set term pngcairo
    set output "~/$d.png"
    plot "~/p.dat", $gd*x
    set output
!
}

for d in `ls -d *`
do
    gd=`ggd`
    echo $gd
    fl=`ls -1 $d/bop/*.bop | awk 'NR>200 && NR % 4 == 0'`
    bop2line o.vtk 64 0.0 0.5 $fl | awk '{print $1 - 0.5*32, $2}' >  ~/p.dat
    gp
done
