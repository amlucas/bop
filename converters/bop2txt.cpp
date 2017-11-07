#include <cstdlib>
#include <cstdio>

#include "bop_common.h"
#include "bop_reader.h"

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
    if (argc < 2) {
        fprintf(stderr, "usage: %s <in1.bop> <in2.bop> ...\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; ++i) {
        BopData d;
    
        init(&d);
        read(argv[i], &d);
        // summary(&d);
    
        switch (d.type) {
        case FLOAT:
        case FASCII:
            float_print(d.fdata, d.n, d.nvars);
            break;
        case DOUBLE:
            float_print(d.ddata, d.n, d.nvars);
            break;
        case INT:
        case IASCII:
            int_print(d.idata, d.n, d.nvars);
            break;
        };
        finalize(&d);
    }    
    return 0;
}

/*

# nTEST: bop2txt.t0
# make 
# ./bop2txt data/test.bop > test.out.txt

*/
