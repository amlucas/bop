# Postprocessing

See also `run*/post`

## Data
====

`daint:/scratch/snx3000/eceva/UDEVICEX/cylinderSDF*`

### List directories

``` Shell
d=/scratch/snx3000/eceva/UDEVICEX/cylinderSDF
ssh daint ls -d $d/'*'
```
### Example

``` Shell
HA=2 RP=8
d=/scratch/snx3000/eceva/UDEVICEX/cylinderSDF
d=$d/cylinderSDF_HA${HA}_RP${RP}
ssh daint ls -d $d/bop/'*.bop'
```


    ./bop2grid o.vtk 10 20 0.0 0.0  0.1 0.1  solvent-00303.bop solvent-00303.bop
