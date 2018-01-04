#include <stdio.h>
#include <stdlib.h>

#include "bop_common.h"
#include "bop_serial.h"
#include "check.h"

int main(int argc, char **argv) {

    BopData *d;
    int n;

    n = 10;

    BPC(bop_ini(&d));
    BPC(bop_set_n(n, d));
    BPC(bop_set_type(BopFASCII, d));
    BPC(bop_set_vars(2, "x y", d));
    BPC(bop_alloc(d));

    BPC(bop_fin(d));
    
    return 0;
}
