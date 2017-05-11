#!/bin/bash

# get data

HA=2 RP=8
d=/scratch/snx3000/eceva/UDEVICEX/cylinderSDF
d=$d/cylinderSDF_HA${HA}_RP${RP}

scp daint:$d/bop/solvent-00303.'*' .
