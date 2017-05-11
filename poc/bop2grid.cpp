#include <cstdlib>
#include <cstdio>

#include "reader.h"
#include "grid2vtk.h"

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

    long  n[] = {10, 20, 30};
    float *org, *sp;
    float *rr, *vv,  *rho;
    grid2vtk(argv[1],
	      n, org, sp,
	     rr, vv, rho);

    for (int i = 0; i < nd; ++i)
    finalize(dd + i);
    finalize(&d);

    delete[] dd;
    
    return 0;
}
