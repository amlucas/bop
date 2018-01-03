#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "bop_serial.h"

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
    long i;
    int d, nf;
    if (nvars < 3) {
        fprintf(stderr, "Need at least 3 coordinates x y z\n");
        exit(1);
    }

    nf = nvars - 3;

    ff = rr = NULL;
        
    if (nf > 0)
    ff = new float[nf * n];
    rr = new float[3  * n];
        
    for (i = 0; i < n; ++i) {
        for (d = 0; d < 3; ++d)
        rr[3*i + d] = (float) data[nvars*i + d];

        for (d = 0; d < nf; ++d)
        ff[n*d + i] = (float) data[nvars*i + 3 + d];
    }

    for (i = 0; i < 3  * n; ++i) rr[i] = EndSwap(rr[i]);
    for (i = 0; i < nf * n; ++i) ff[i] = EndSwap(ff[i]);
}

void init_i(const long n, const int nvars, const int *data) {
    long i;
    int d;
    ii = NULL;
        
    if (nvars > 0)
    ii = new int[nvars * n];
        
    for (i = 0; i < n; ++i)
        for (d = 0; d < nvars; ++d)
            ii[nvars*d + i] = data[nvars*i + d];

    for (i = 0; i < nvars * n; ++i) ii[i] = EndSwap(ii[i]);
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
} // vtk

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s <out.vtk> <in1.bop> <in2.bop> ... -- <in1.int.bop> <in2.int.bop> ...\n", argv[0]);
        exit(1);
    }
    int i_int, i, ninput, nd;
    BopData *fdd, *idd, d, di;
    char dfname[CBUFSIZE];
    
    i_int = -1;
    for (i = 2; i < argc; ++i) if (strcmp(argv[i], "--") == 0) i_int = i + 1; 

    const bool read_int = i_int != -1;
    
    ninput = argc-2;
    nd = read_int ? (ninput - 1) / 2 : ninput;
    
    fdd = new BopData[nd];
    idd = new BopData[nd];

    for (i = 0; i < nd; ++i) {
        bop_read_header(argv[2+i], /**/ fdd + i, dfname);
        bop_alloc(fdd + i);
        bop_read_values(dfname, /**/ fdd + i);
        
        if (read_int) {
            bop_read_header(argv[i_int+i], /**/ idd + i, dfname);
            bop_alloc(idd + i);
            bop_read_values(dfname, /**/ idd + i);
        }
    }

    bop_concatenate(nd, fdd, /**/ &d);
    if (read_int) bop_concatenate(nd, idd, /**/ &di);

    // bop_summary(&d);
    // if (read_int) bop_summary(&di);
        
    FILE *f = fopen(argv[1], "w");
    
    switch (d.type) {
    case BopData::FLOAT:
    case BopData::FASCII:
        vtk::init(d.n, d.nvars, (const float*) d.data);
        break;
    case BopData::DOUBLE:
        vtk::init(d.n, d.nvars, (const double *) d.data);
        break;
    case BopData::INT:
    case BopData::IASCII:
        break;
    };

    if (read_int) vtk::init_i(di.n, di.nvars, (const int *) di.data);

    Cbuf *vars, *ivars;
    vars = ivars = NULL;
    
    vars = new Cbuf[d.nvars];
    bop_extract_vars(&d, /**/ vars);

    if (read_int) {
        ivars = new Cbuf[di.nvars];
        bop_extract_vars(&di, /**/ ivars);
    }
    
    vtk::header  (f, d.n);
    vtk::vertices(f, d.n);
    vtk::fields  (f, d.n, d.nvars, vars);
    if (read_int) vtk::ifields(f, di.n, di.nvars, ivars);
    vtk::finalize();

    fclose(f);

    for (i = 0; i < nd; ++i) {
        bop_free(fdd + i);
        if (read_int)
            bop_free(idd + i);
    }
    bop_free(&d);
    delete[] vars;

    if (read_int) {
        bop_free(&di);
        delete[] ivars;
    }
    
    delete[] fdd;
    delete[] idd;
    
    return 0;
}

/*

  # nTEST: bop2vtk.t0
  # make 
  # ./bop2vtk test.out.vtk data/test.bop

*/
