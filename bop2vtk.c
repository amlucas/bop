#include <stdio.h>
#include "endian.h"

#define endswap(e) FloatSwap((e)) /* change to have different type */
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

long read0(FILE* f) { /* sets `nfpp' */
  /* [r]ead to [b]uffer */
  int n0; long n; /* TODO: should be long in udx */
  fread(&n0, 1, sizeof(n0), f); n = n0;
  nfpp  = nreal(f) / n;
  fprintf(stderr, "(bop2vtk) n, nfpp: %ld %d\n", n, nfpp);
  fread(ibuf, n*nfpp, sizeof(ibuf[0]), f);
  return n;
}

long read(const char* fn) {
  fprintf(stderr, "(bop2vtk) reading: %s\n", fn);
  FILE* f = fopen(fn, "r");
  long n = read0(f);
  fclose(f);
  return n;
}

void buf2fields(long n) {
  long i, j, dim = 3;
  for (i = 0; i < n; i++) {
    j = 0;
    for (/**/; j < dim;  j++)
      obuf[3*i + j] = endswap(ibuf[i*nfpp + j]);
    for (/**/; j < nfpp; j++)
      obuf[j*n + i] = endswap(ibuf[i*nfpp + j]);
  }
}

void version(void) {PR("# vtk DataFile Version 2.0\n");}
void header(void) {PR("Created with bop2vtk\n");}
void format(void) {PR("BINARY\n");}
void vertices(real** pbuf, long n) {
  PR("DATASET POLYDATA\n");
  PR("POINTS %ld %s\n", n, dataType);
  int sz = sizeof(*pbuf[0]);
  fwrite(*pbuf, 3*n, sz, fo); *pbuf += 3*n;
  PR("\n");
}

void attributes_header(long n) {PR("POINT_DATA %ld\n", n);}

void attribute(const char *name, real **pbuf, long n) {
  PR("SCALARS %s %s\n", name, dataType);
  PR("LOOKUP_TABLE default\n");
  int sz = sizeof(*pbuf[0]);
  fwrite(*pbuf, n, sz, fo); *pbuf += n;
}

void write0(long n) {
  real* buf = obuf;
  version();
  header();
  format();
  vertices(&buf, n);
  attributes_header(n);

  int ia, na = sizeof ANAMES; /* number of attributes */
  for (ia = 0; ia < na && ia + 3 < nfpp; ia++)
    attribute(ANAMES[ia], &buf, n);
}

void write(const char* fn, long n) {
  fprintf(stderr, "(bop2vtk) writing: %s\n", fn);
  fo = fopen(fn, "w");
  
  write0(n);
  
  fclose(fo);
}

int main(int argc, char *argv[]) {
  int iarg = 1;
  char *out = argv[iarg++];

  long n = 0; /* number of particles */
  while (iarg < argc) n += read(argv[iarg++]);

  buf2fields(n);
  write(out, n);
  return 0;
}
