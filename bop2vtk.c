#include <stdio.h>
#include "endian.h"

#define MAX_PART_NUM 1000000
#define MAX_LINE     15     /* max number of fields per particle */

#define endswap(e) FloatSwap((e))
#define dataType "float"
typedef float real;

real fbuf[MAX_LINE*MAX_PART_NUM];

real rr[3*MAX_PART_NUM];
real vvx[MAX_PART_NUM], vvy[MAX_PART_NUM], vvz[MAX_PART_NUM];

int nfpp; /* number of fields per particle */

FILE* fo;  /* output file descriptor */
#define pr(...) fprintf(fo, __VA_ARGS__)

/* [s]wap [f]loat and put it into buffer */
#define sf(fl) fbuf[ib++] = endswap((fl))

/* [w]rite buffer to a file*/
#define wb(b) fwrite((b), (ib), sizeof((b)[0]), fo)


long nflo(FILE* fd) { /* return a number reals in file */
  long end, curr;
  curr = ftell(fd);
  fseek(fd, 0, SEEK_END); end = ftell(fd);
  fseek(fd, curr, SEEK_SET); /* go back */
  return (end - curr)/sizeof(real);
}

long read_file0(FILE* f) { /* sets `nfpp' */
  /* [r]ead to [b]uffer */
#define rb(b, n) fread((b), (n), sizeof((b)[0]), f)
  int n0; long n;
  fread(&n0, 1, sizeof(n0), f); n = n0;
  
  nfpp  = nflo(f) / n;
  fprintf(stderr, "(bop2vtk) n, nfpp: %ld %d\n", n, nfpp);
  rb(fbuf, n*nfpp);
#undef rb
  return n;
}

long read_file(const char* fn) {
  fprintf(stderr, "(bop2vtk) reading: %s\n", fn);
  FILE* f = fopen(fn, "r");
  long n = read_file0(f);
  fclose(f);
  return n;
}

void buf2fields(long n) {
  enum {X, Y, Z};
  long i, ib;
  for (i = 0; i < n; i++) {
    ib = nfpp * i;
    real *r = &rr[3*i];
    r[X]   = fbuf[ib++]; r[Y]   = fbuf[ib++]; r[Z]   = fbuf[ib++];
    vvx[i] = fbuf[ib++]; vvy[i] = fbuf[ib++]; vvz[i] = fbuf[ib++];
  }
}

void write_version(void) {pr("# vtk DataFile Version 2.0\n");}
void write_header(void) {pr("Created with bop2vtk\n");}
void write_format(void) {pr("BINARY\n");}
void write_vertices(long n) {
  pr("DATASET POLYDATA\n");
  pr("POINTS %ld %s\n", n, dataType);
  long i, ib = 0;
  for (i = 0; i < n; i++) {
    sf(rr[3*i]); sf(rr[3*i+1]); sf(rr[3*i+2]);
  }
  wb(fbuf); /* write buffer */
  pr("\n");
}

void write_attributes_header(long n) {pr("POINT_DATA %ld\n", n);}

void write_attribute(const char *name, real *data, long n) {
  pr("SCALARS %s %s\n", name, dataType);
  pr("LOOKUP_TABLE default\n");
  long i, ib = 0;
  for (i = 0; i < n; i++) sf(data[i]);
  wb(fbuf);
}
void write_file(const char* fn, long n) {
  fprintf(stderr, "(bop2vtk) writing: %s\n", fn);
  fo = fopen(fn, "w");
  write_version();
  write_header();
  write_format();
  write_vertices(n);
  write_attributes_header(n);

  write_attribute("vx", vvx, n);
  write_attribute("vy", vvy, n);
  write_attribute("vz", vvz, n);

  fclose(fo);
}

int main(int argc, char *argv[]) {
  long n = read_file(argv[2]);
  buf2fields(n);

  write_file(argv[1], n);
  return 0;
}
