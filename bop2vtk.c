#include <stdio.h>
#include "endian.h"

#define MAX_PART_NUM 1000000
#define nfpp   6     /* number of fields per particle */

float fbuf[nfpp*MAX_PART_NUM];
int ib; /* position in the buffer */

#define dataType "float"

float rr[3*MAX_PART_NUM];
float vvx[MAX_PART_NUM], vvy[MAX_PART_NUM], vvz[MAX_PART_NUM];

int n; /* number of particles */

FILE* fo;  /* output file descriptor */
#define pr(...) fprintf(fo, __VA_ARGS__)

/* [s]wap [f]loat and put it into buffer */
void sf(float fl) { fbuf[ib++] = FloatSwap(fl);}

/* [w]rite buffer to a file*/
#define wb(b) fwrite((b), (ib), sizeof((b)[0]), fo)

/* [r]ead to buffer */
#define rb(b, n) fread((b), (n), sizeof((b)[0]), fi)

void read_file(const char* fn) {
  fprintf(stderr, "(bop2vtk) reading: %s\n", fn);
  FILE* fi = fopen(fn, "r");
  fread(&n, 1, sizeof(n), fi);
  fprintf(stderr, "(bop2vtk) n: %d\n", n);
  rb(fbuf, n*nfpp);
  fclose(fi);
}

void buf2fields(void) {
  int ir = 0, iv = 0;
  ib = 0;
  while (ib < n*nfpp) {
    rr[ir++] = fbuf[ib++]; rr[ir++] = fbuf[ib++]; rr[ir++] = fbuf[ib++];
    vvx[iv] = fbuf[ib++]; vvy[iv] = fbuf[ib++]; vvz[iv] = fbuf[ib++]; iv++;
  }
}

void write_version(void) {pr("# vtk DataFile Version 2.0\n");}
void write_header(void) {pr("Created with vrbc bop2vtk\n");}
void write_format(void) {pr("BINARY\n");}
void write_vertices(void) {
  pr("DATASET POLYDATA\n");
  pr("POINTS %d %s\n", n, dataType);
  int i;
  ib = 0;
  for (i = 0; i < n; i++) {
    sf(rr[3*i]); sf(rr[3*i+1]); sf(rr[3*i+2]);
  }
  wb(fbuf); /* write buffer */
  pr("\n");
}

void write_attributes_header(void) {pr("POINT_DATA %d\n", n);}

void write_attribute(const char *name, float *data) {
  pr("SCALARS %s %s\n", name, dataType);
  pr("LOOKUP_TABLE default\n");
  int i; ib = 0;
  for (i = 0; i < n; i++) sf(data[i]);
  wb(fbuf);
}
void write_file(const char* fn) {
  fprintf(stderr, "(bop2vtk) writing: %s\n", fn);
  fo = fopen(fn, "w");
  write_version();
  write_header();
  write_format();
  write_vertices();
  write_attributes_header();

  write_attribute("dx", vvx);
  write_attribute("dy", vvy);
  write_attribute("dz", vvz);

  fclose(fo);
}

int main(int argc, char *argv[]) {
  read_file(argv[1]);
  buf2fields();

  write_file(argv[2]);
  return 0;
}
