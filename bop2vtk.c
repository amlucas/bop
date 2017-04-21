#include <stdio.h>
#include "endian.h"

#define endswap(e) FloatSwap((e))
#define dataType "float"
typedef float real;

char* ANAMES[] = {"vx", "vy", "vz", "id", "p"};

#define MAX_PART_NUM 1000000
#define MAX_LINE     15     /* max number of fields per particle */

real ibuf[MAX_LINE*MAX_PART_NUM]; /* input and output buffers */
real obuf[MAX_LINE*MAX_PART_NUM];

int nfpp; /* number of fields per particle */

FILE* fo;  /* output file descriptor */
#define PR(...) fprintf(fo, __VA_ARGS__)

long nreal(FILE* fd) { /* return a number of reals */
  long end, curr;
  curr = ftell(fd);
  fseek(fd, 0, SEEK_END); end = ftell(fd);
  fseek(fd, curr, SEEK_SET); /* go back */
  return (end - curr)/sizeof(real);
}

long read_file0(FILE* f) { /* sets `nfpp' */
  /* [r]ead to [b]uffer */
  int n0; long n; /* TODO: should be long in udx */
  fread(&n0, 1, sizeof(n0), f); n = n0;
  nfpp  = nreal(f) / n;
  fprintf(stderr, "(bop2vtk) n, nfpp: %ld %d\n", n, nfpp);
  fread(ibuf, n*nfpp, sizeof(ibuf[0]), f);
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
  int i, j, dim = 3;
  for (i = 0; i < n; i++) {
    j = 0;
    for (/**/; j < dim;  j++)
      obuf[3*i + j] = endswap(ibuf[i*nfpp + j]);
    for (/**/; j < nfpp; j++)
      obuf[j*n + i] = endswap(ibuf[i*nfpp + j]);
  }
}

void write_version(void) {PR("# vtk DataFile Version 2.0\n");}
void write_header(void) {PR("Created with bop2vtk\n");}
void write_format(void) {PR("BINARY\n");}
void write_vertices(real** pbuf, long n) {
  PR("DATASET POLYDATA\n");
  PR("POINTS %ld %s\n", n, dataType);
  int sz = sizeof(*pbuf[0]);
  fwrite(*pbuf, 3*n, sz, fo); *pbuf += 3*n;
  PR("\n");
}

void write_attributes_header(long n) {PR("POINT_DATA %ld\n", n);}

void write_attribute(const char *name, real **pbuf, long n) {
  PR("SCALARS %s %s\n", name, dataType);
  PR("LOOKUP_TABLE default\n");
  int sz = sizeof(*pbuf[0]);
  fwrite(*pbuf, n, sz, fo); *pbuf += n;
}
void write_file(const char* fn, long n) {
  real* buf = obuf;

  fprintf(stderr, "(bop2vtk) writing: %s\n", fn);
  fo = fopen(fn, "w");
  write_version();
  write_header();
  write_format();
  write_vertices(&buf, n);
  write_attributes_header(n);

  int ia, na = sizeof ANAMES; /* number of attributes */
  for (ia = 0; ia < na && ia + 3 < nfpp; ia++)
    write_attribute(ANAMES[ia], &buf, n);

  fclose(fo);
}

int main(int argc, char *argv[]) {
  int iarg = 1;
  char *out = argv[iarg++];

  long n = 0; /* number of particles */
  while (iarg < argc) n += read_file(argv[iarg++]);

  buf2fields(n);
  write_file(out, n);
  return 0;
}
