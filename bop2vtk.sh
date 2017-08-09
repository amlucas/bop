# !/bin/bash
# 
# Convert multiple .bop files to .vtk files
# run: ./<script_name> <rundirs>

# Loop over different simulation directories
for d; do
    o=$d/vtk
    mkdir -p $o

    files=`ls $d/bop/*.bop`
    
    for ff in $files; do
        b=`basename $ff`
        bop2vtk ${o}/${b}.vtk ${ff}
    done
done
