
for g in 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 11.0 12.0
do
    f=`./r3.m ~/s/sh_$g/ply/rbcs-01*.ply`
    echo $g $f
done
