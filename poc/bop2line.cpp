#include <cstdlib>
#include <cstdio>
#include "reader.h"

int n;    /* number of steps */
double o; /* origin */
double s; /* spacing */

char *fo; /* output file */

int main(int argc, char **argv) {
  int iarg = 1;
  /* fprintf(stderr, "usage: %s <out.vtk> <n> <o> <s>  <in1.bop> <in2.bop> ...\n", argv[iarg++]); */
  fo =      argv[iarg++];

  n = atoi(argv[iarg++]);
  o = atof(argv[iarg++]);
  s = atof(argv[iarg++]);

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
}
