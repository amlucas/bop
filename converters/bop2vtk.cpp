#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_reader.h"

template <typename T>
T EndSwap(T f) {
    static_assert(sizeof(T) == 4 * sizeof(unsigned char), "wrong type: must have 4 bytes");
    union {
        T f;
        unsigned char b[4];
    } dat1, dat2;

    dat1.f = f;
    dat2.b[0] = dat1.b[3];
    dat2.b[1] = dat1.b[2];
    dat2.b[2] = dat1.b[1];
    dat2.b[3] = dat1.b[0];
    return dat2.f;
}

namespace vtk {
float *rr, *ff; /* positions, fields */
int *ii;        /* integer fields */
    
template <typename T>
void init(const long n, const int nvars, const T *data) {
    if (nvars < 3) {
        fprintf(stderr, "Need at least 3 coordinates x y z\n");
        exit(1);
    }

    const int nf = nvars - 3;

    ff = rr = NULL;
        
    if (nf > 0)
    ff = new float[nf * n];
    rr = new float[3  * n];
        
    for (long i = 0; i < n; ++i) {
        for (int d = 0; d < 3; ++d)
        rr[3*i + d] = (float) data[nvars*i + d];

        for (int d = 0; d < nf; ++d)
        ff[n*d + i] = (float) data[nvars*i + 3 + d];
    }

    for (long i = 0; i < 3  * n; ++i) rr[i] = EndSwap(rr[i]);
    for (long i = 0; i < nf * n; ++i) ff[i] = EndSwap(ff[i]);
}

void init_i(const long n, const int nvars, const int *data) {
    ii = NULL;
        
    if (nvars > 0)
    ii = new int[nvars * n];
        
    for (long i = 0; i < n; ++i)
    for (int d = 0; d < nvars; ++d)
    ii[nvars*d + i] = data[nvars*i + d];

    for (long i = 0; i < nvars * n; ++i) ii[i] = EndSwap(ii[i]);
}

void finalize() {
    if (rr) delete[] rr;
    if (ff) delete[] ff;
    if (ii) delete[] ii;
}
    
void header(FILE *f, const long n) {
    fprintf(f, "# vtk DataFile Version 2.0\n");
    fprintf(f, "created with bop2vtk\n");
    fprintf(f, "BINARY\n");
}

void vertices(FILE *f, const long n) {
    fprintf(f, "DATASET POLYDATA\n");
    fprintf(f, "POINTS %ld float\n", n);
    fwrite(rr, 3*n, sizeof(float), f);
    fprintf(f, "\n");
}

void fields(FILE *f, const long n, const int nvars, const Cbuf *vars) {
    if (nvars <= 3) return;

    fprintf(f, "POINT_DATA %ld\n", n);

    for (int i = 3; i < nvars; ++i) {
        fprintf(f, "SCALARS %s float\n", vars[i].c);
        fprintf(f, "LOOKUP_TABLE default\n");
        fwrite(ff + (i-3)*n, n, sizeof(float), f);
    }
}

void ifields(FILE *f, const long n, const int nvars, const Cbuf *vars) {
    if (nvars <= 0) return;

    for (int i = 0; i < nvars; ++i) {
        fprintf(f, "SCALARS %s int\n", vars[i].c);
        fprintf(f, "LOOKUP_TABLE default\n");
        fwrite(ii + i * n, n, sizeof(int), f);
    }
}
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <out.vtk> <in1.bop> <in2.bop> ... -- <in1.int.bop> <in2.int.bop> ...\n", argv[0]);
        exit(1);
    }
    int i_int, i, ninput, nd;
    BopData *fdd, *idd, d, di;
    
    i_int = -1;
    for (i = 2; i < argc; ++i) if (strcmp(argv[i], "--") == 0) i_int = i + 1; 

    const bool read_int = i_int != -1;
    
    ninput = argc-2;
    nd = read_int ? (ninput - 1) / 2 : ninput;
    
    fdd = new BopData[nd];
    idd = new BopData[nd];

    for (i = 0; i < nd; ++i) {
        init(fdd + i);
        read(argv[2+i], fdd + i);

        if (read_int) {
            init(idd + i);
            read(argv[i_int+i], idd + i);
        }
    }

    init(&d); init(&di);
    concatenate(nd, fdd, /**/ &d);
    if (read_int) concatenate(nd, idd, /**/ &di);

    // summary(&d);
    // if (read_int) summary(&di);
        
    FILE *f = fopen(argv[1], "w");
    
    switch (d.type) {
    case FLOAT:
    case FASCII:
        vtk::init(d.n, d.nvars, d.fdata);
        break;
    case DOUBLE:
        vtk::init(d.n, d.nvars, d.ddata);
        break;
    case INT:
    case IASCII:
        break;
    };

    if (read_int) vtk::init_i(di.n, di.nvars, di.idata);
    
    vtk::header  (f, d.n);
    vtk::vertices(f, d.n);
    vtk::fields  (f, d.n, d.nvars, d.vars);
    if (read_int) vtk::ifields(f, di.n, di.nvars, di.vars);
    vtk::finalize();

    fclose(f);

    for (i = 0; i < nd; ++i) {
        finalize(fdd + i);
        if (read_int) finalize(idd + i);
    }
    finalize(&d);
    finalize(&di);

    delete[] fdd;
    delete[] idd;
    
    return 0;
}

/*

  # nTEST: bop2vtk.t0
  # make 
  # ./bop2vtk test.out.vtk data/test.bop

*/
