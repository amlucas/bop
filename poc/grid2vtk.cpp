#include "grid2vtk.h"
#include <stdio.h>

static FILE *f;
static long np;
enum {X, Y, Z};
#define pr(...) fprintf(f, __VA_ARGS__)

static void header() {
  pr("# vtk DataFile Version 2.0\n");
  pr("created with bop2grid\n");
  /*   pr("BINARY\n"); */
  pr("ASCII\n");
}

static void topology (long *nd, double *org, double *sp) {
  pr("DATASET STRUCTURED_POINTS\n");
  pr("DIMENSIONS %ld %ld %ld\n", nd[X], nd[Y],  nd[Z]);
  pr("ORIGIN  %.9g %.9g %.9g\n", org[X],org[Y], org[Z]);
  pr("SPACING %.9g %.9g %.9g\n", sp[X], sp[Y],  sp[Z]);
}

static void data_header() {pr("POINT_DATA %ld\n", np);}

static void scalar(double* sc, const char name[]) {
  pr("SCALARS %s double\n", name);
  pr("LOOKUP_TABLE default\n");
  /* fwrite(sc, np, sizeof(sc[0]), f); */
  for (long i = 0; i < np; i++) pr("%g\n", sc[i]);
}

void grid2vtk(const char *fn,
	      long   nd[3], double org[3], double sp[3],
	      double *vars[], char const *names[], int nvar) {
  f = fopen(fn, "w");

  np = nd[X]*nd[Y]*nd[Z];
  header();
  topology(nd, org, sp);
  data_header();
  for (int i = 0; i < nvar; i++) scalar(vars[i], names[i]);  
  fclose(f);
}
