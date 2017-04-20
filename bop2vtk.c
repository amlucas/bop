#include <stdio.h>
#include "endian.h"

#define MAX_PART_NUM 1000000
#define MAX_LINE     15     /* max number of fields per particle */

float fbuf[MAX_LINE*MAX_PART_NUM];

#define dataType "float"

float rr[3*MAX_PART_NUM];
float vvx[MAX_PART_NUM], vvy[MAX_PART_NUM], vvz[MAX_PART_NUM];

int n; /* number of particles */
int nfpp; /* number of fields per particle */

FILE* fo;  /* output file descriptor */
#define pr(...) fprintf(fo, __VA_ARGS__)

/* [s]wap [f]loat and put it into buffer */
#define sf(fl) fbuf[ib++] = FloatSwap((fl))

/* [w]rite buffer to a file*/
#define wb(b) fwrite((b), (ib), sizeof((b)[0]), fo)


long nflo(FILE* fd) { /* return a number floats in file */
  long end, curr;
  curr = ftell(fd);
  fseek(fd, 0, SEEK_END); end = ftell(fd);
  fseek(fd, curr, SEEK_SET); /* go back */
  return (end - curr)/sizeof(float);
}

void read_file0(FILE* f) { /* sets `n' and `nfpp' */
  /* [r]ead to [b]uffer */
  #define rb(b, n) fread((b), (n), sizeof((b)[0]), f)
  fread(&n, 1, sizeof(n), f);
  nfpp  = nflo(f) / n;
  fprintf(stderr, "(bop2vtk) n, nfpp: %d %d\n", n, nfpp);
  rb(fbuf, n*nfpp);
  #undef rb
}

void read_file(const char* fn) {
  fprintf(stderr, "(bop2vtk) reading: %s\n", fn);
  FILE* f = fopen(fn, "r");
  read_file0(f);
  fclose(f);
}

void buf2fields(void) {
  enum {X, Y, Z};
  long i, ib;
  for (i = 0; i < n; i++) {
    ib = nfpp * i;
    float *r = &rr[3*i];
    r[X]   = fbuf[ib++]; r[Y]   = fbuf[ib++]; r[Z]   = fbuf[ib++];
    vvx[i] = fbuf[ib++]; vvy[i] = fbuf[ib++]; vvz[i] = fbuf[ib++];
  }
}

void write_version(void) {pr("# vtk DataFile Version 2.0\n");}
void write_header(void) {pr("Created with bop2vtk\n");}
void write_format(void) {pr("BINARY\n");}
void write_vertices(void) {
  pr("DATASET POLYDATA\n");
  pr("POINTS %d %s\n", n, dataType);
  long i, ib = 0;
  for (i = 0; i < n; i++) {
    sf(rr[3*i]); sf(rr[3*i+1]); sf(rr[3*i+2]);
  }
  wb(fbuf); /* write buffer */
  pr("\n");
}

void write_attributes_header(void) { pr("POINT_DATA %d\n", n); }

void write_attribute(const char *name, float *data) {
  pr("SCALARS %s %s\n", name, dataType);
  pr("LOOKUP_TABLE default\n");
  long i, ib = 0;
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

  write_attribute("vx", vvx);
  write_attribute("vy", vvy);
  write_attribute("vz", vvz);

  fclose(fo);
}

int main(int argc, char *argv[]) {
  read_file(argv[2]);
  buf2fields();

  write_file(argv[1]);
  return 0;
}
