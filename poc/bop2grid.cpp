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

int nx, ny;
double ox, oy;
double sx, sy;

char *fo; /* output file */

int main(int argc, char **argv) {
  int iarg = 1;
  /* fprintf(stderr, "usage: %s <out.vtk> <nx> <ny> <ox> <oy> <sx> <sy> <in1.bop> <in2.bop> ...\n", argv[iarg++]); */
  fo =      argv[iarg++];

  nx = atoi(argv[iarg++]);
  ny = atoi(argv[iarg++]);

  ox = atof(argv[iarg++]);
  oy = atof(argv[iarg++]);

  sx = atof(argv[iarg++]);
  sy = atof(argv[iarg++]);

  const int nd = argc - iarg;
  printf("nd: %d\n", nd);
  ReadData *dd = new ReadData[nd];

  for (int i = 0; i < nd; i++, iarg++) {
    init(dd + i);
    read(argv[iarg], dd + i);
  }

  ReadData d;
  init(&d);
  concatenate(nd, dd, /**/ &d);
  summary(&d);

  long     n[] = {nx, ny, 1};
  double org[] = {ox, oy, 0};
  double sp[]  = {sx, sy, 0};
  double *vy, *vx;
  char   const *nname[] = {"vx", "vy"};
  int    ns = sizeof nname / sizeof nname[0];

  long np = nx * ny;
  vx = (double*)malloc(np*sizeof vx[0]);
  vy = (double*)malloc(np*sizeof vy[0]);
  for (long i = 0; i < np; i++) vx[i] = -i;
  for (long i = 0; i < np; i++) vy[i] =  i;
  double *vars[] = {vx, vy};
  grid2vtk(fo,
	   n, org, sp,
	   vars, nname, ns);

  for (int i = 0; i < nd; ++i) finalize(dd + i);
  finalize(&d);
  delete[] dd;
}
