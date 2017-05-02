#include <cstdlib>
#include <cstdio>

#include "reader.h"

template <typename real>
void print(const real *data, const long n, const int nvars)
{
    for (long i = 0; i < n; ++i)
    {
        for (int j = 0; j < nvars; ++j)
        printf("%.6e ", data[nvars*i + j]);
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <in1.bop> <in2.bop> ...\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; ++i)
    {
        ReadData d;
    
        init(&d);
        read(argv[i], &d);
        summary(&d);
    
        switch (d.type)
        {
        case FLOAT:
        case ASCII:
            print(d.fdata, d.n, d.nvars);
            break;
        case DOUBLE:
            print(d.ddata, d.n, d.nvars);
            break;
        };
    
        finalize(&d);
    }
    
    return 0;
}
