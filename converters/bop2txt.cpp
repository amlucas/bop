#include <cstdlib>
#include <cstdio>

#include "bop_common.h"
#include "bop_serial.h"

template <typename real>
void float_print(const real *data, const long n, const int nvars) {
    for (long i = 0; i < n; ++i) {
        for (int j = 0; j < nvars; ++j)
        printf("%.6e ", data[nvars*i + j]);
        printf("\n");
    }
}

void int_print(const int *data, const long n, const int nvars) {
    for (long i = 0; i < n; ++i) {
        for (int j = 0; j < nvars; ++j)
        printf("%d ", data[nvars*i + j]);
        printf("\n");
    }
}

int main(int argc, char **argv) {
    char dfname[256];
    BopData *d;
    BopType type;
    int i, nvars;
    long n;
    if (argc < 2) {
        fprintf(stderr, "usage: %s <in1.bop> <in2.bop> ...\n", argv[0]);
        exit(1);
    }

    for (i = 1; i < argc; ++i) {

        bop_ini(&d);
        bop_read_header(argv[i], /**/ d, dfname);
        bop_alloc(d);
        bop_read_values(dfname, /**/ d);
        // bop_summary(d);

        bop_get_type(d, &type);
        bop_get_n(d, &n);
        bop_get_nvars(d, &nvars);
        
        switch (type) {
        case BopFLOAT:
        case BopFASCII:
            float_print((const float *) bop_get_data(d), n, nvars);
            break;
        case BopDOUBLE:
            float_print((const double *) bop_get_data(d), n, nvars);
            break;
        case BopINT:
        case BopIASCII:
            int_print((const int *) bop_get_data(d), n, nvars);
            break;
        };
        bop_fin(d);
    }    
    return 0;
}

/*

# nTEST: bop2txt.t0
# make 
# ./bop2txt data/test.bop > test.out.txt

*/
