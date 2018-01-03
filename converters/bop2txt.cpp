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
    char dfname[CBUFSIZE];
    if (argc < 2) {
        fprintf(stderr, "usage: %s <in1.bop> <in2.bop> ...\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; ++i) {
        BopData d;

        bop_read_header(argv[i], /**/ &d, dfname);
        bop_alloc(&d);
        bop_read_values(dfname, /**/ &d);
        // summary(&d);
    
        switch (d.type) {
        case BopData::FLOAT:
        case BopData::FASCII:
            float_print((const float *) d.data, d.n, d.nvars);
            break;
        case BopData::DOUBLE:
            float_print((const double *) d.data, d.n, d.nvars);
            break;
        case BopData::INT:
        case BopData::IASCII:
            int_print((const int *) d.data, d.n, d.nvars);
            break;
        };
        bop_free(&d);
    }    
    return 0;
}

/*

# nTEST: bop2txt.t0
# make 
# ./bop2txt data/test.bop > test.out.txt

*/
