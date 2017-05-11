#include "grid2vtk.h"
#include <stdio.h>

#define pr(...) fprintf(f, __VA_ARGS__)
static FILE *f;

static void header() {
  pr("# vtk DataFile Version 2.0\n");
  pr("created with bop2grid\n");
  pr("BINARY\n");
}

static void topology (long   *n, double *org, double *sp) {
  enum {X, Y, Z};
  pr("STRUCTURED_POINTS\n");
  pr("DIMENSIONS %ld %ld %ld\n", n[X], n[Y], n[Z]);
  pr("ORIGIN %.9g %.9g %.9g\n", org[X], org[Y], org[Z]);
  pr("SPACING %.9g %.9g %.9g\n", sp[X], sp[Y], sp[Z]);
}

void grid2vtk(const char *fn,
	      long   *n, double *org, double *sp,
	      double *vv,  double *rho) {
  f = fopen(fn, "w");
  
  header();
  topology(n, org, sp);
  
  fclose(f);
}
