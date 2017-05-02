#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "reader.decl.h"
#include "reader.impl.h"

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
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <in.bop>\n", argv[0]);
        exit(1);
    }

    reader::init();
    reader::read(argv[1]);
    reader::summary();
    
    switch (reader::type)
    {
    case FLOAT:
    case ASCII:
        print(reader::fdata, reader::n, reader::nvars);
        break;
    case DOUBLE:
        print(reader::ddata, reader::n, reader::nvars);
        break;
    };
    
    reader::finalize();
    
    return 0;
}
