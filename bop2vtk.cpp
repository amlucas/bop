#include <cstdlib>
#include <cstdio>

#include "reader.h"

float FloatSwap(float f) {
  union {
    float f;
    unsigned char b[4];
  } dat1, dat2;

  dat1.f = f;
  dat2.b[0] = dat1.b[3];
  dat2.b[1] = dat1.b[2];
  dat2.b[2] = dat1.b[1];
  dat2.b[3] = dat1.b[0];
  return dat2.f;
}

namespace vtk
{
    float *vv, *ff;
    
    template <typename T>
    void init(const long n, const int nvars, const T *data)
    {
        if (nvars < 3)
        {
            fprintf(stderr, "Need at least 3 coordinates x y z\n");
            exit(1);
        }

        const int nf = nvars - 3;

        ff = vv = NULL;
        
        if (nf > 0)
        ff = new float[nf * n];
        vv = new float[3  * n];
        
        for (long i = 0; i < n; ++i)
        {
            for (int d = 0; d < 3; ++d)
            vv[3*i + d] = (float) data[nvars*i + d];

            for (int d = 0; d < nf; ++d)
            ff[n*d + i] = (float) data[nvars*i + 3 + d];
        }

        for (long i = 0; i < 3  * n; ++i) vv[i] = FloatSwap(vv[i]);
        for (long i = 0; i < nf * n; ++i) ff[i] = FloatSwap(ff[i]);
    }

    void finalize()
    {
        if (vv) delete[] vv;
        if (ff) delete[] ff;
    }
    
    void header(FILE *f, const long n)
    {
        fprintf(f, "# vtk DataFile Version 2.0\n");
        fprintf(f, "created with bop2vtk\n");
        fprintf(f, "BINARY\n");
    }

    void vertices(FILE *f, const long n)
    {
        fprintf(f, "DATASET POLYDATA\n");
        fprintf(f, "POINTS %ld float\n", n);
        fwrite(vv, 3*n, sizeof(float), f);
        fprintf(f, "\n");
    }

    void fields(FILE *f, const long n, const int nvars, const Cbuf *vars)
    {
        if (nvars <= 3) return;

        fprintf(f, "POINT_DATA %ld\n", n);

        for (int i = 3; i < nvars; ++i)
        {
            fprintf(f, "SCALARS %s float\n", vars[i].c);
            fprintf(f, "LOOKUP_TABLE default\n");
            fwrite(ff + (i-3)*n, n, sizeof(float), f);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: %s <out.vtk> <in1.bop> <in2.bop> ...\n", argv[0]);
        exit(1);
    }
    const int nd = argc-2;
    ReadData *dd = new ReadData[nd];

    for (int i = 0; i < nd; ++i)
    {
        init(dd + i);
        read(argv[2+i], dd + i);
    }

    ReadData d;
    init(&d);
    concatenate(nd, dd, /**/ &d);

    summary(&d);
        
    FILE *f = fopen(argv[1], "w");
    
    switch (d.type)
    {
    case FLOAT:
    case ASCII:
        vtk::init(d.n, d.nvars, d.fdata);
        break;
    case DOUBLE:
        vtk::init(d.n, d.nvars, d.ddata);
        break;
    };

    vtk::header  (f, d.n);
    vtk::vertices(f, d.n);
    vtk::fields  (f, d.n, d.nvars, d.vars);
    vtk::finalize();

    fclose(f);

    for (int i = 0; i < nd; ++i)
    finalize(dd + i);
    finalize(&d);

    delete[] dd;
    
    return 0;
}

/*

# nTEST: bop2vtk.t0
# make 
# ./bop2vtk test.out.vtk data/test.bop

*/
