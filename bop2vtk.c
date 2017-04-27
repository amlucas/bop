#include <stdio.h>
#include <stdlib.h>
#include "endian.h"

typedef float real;
#include "bop.utils.h"
#define endswap0(e) FloatSwap((e)) /* change to have different type */
#define dataType "float"

char *ANAMES[] = {"vx", "vy", "vz", "id", "p"};

#define MAX_PART_NUM 1000000
#define MAX_LINE     15     /* max number of fields per particle */

real ibuf[MAX_LINE*MAX_PART_NUM]; /* input and output buffers */
real obuf[MAX_LINE*MAX_PART_NUM];

int nrpp; /* [n]umber of [r]eals [p]er [p]article */
FILE* fo;  /* output file descriptor */

void usage(void) { fprintf(stderr, "usage: bop2vtk output.file input.file [input.file]\n"); }
void die(void)   { usage(); exit(1); }

#define PR(...) fprintf(fo, __VA_ARGS__)
void version(void) {PR("# vtk DataFile Version 2.0\n");}
void header(void) {PR("Created with bop2vtk\n");}
void format(void) {PR("BINARY\n");}
void vertices(real **pbuf, long n) {
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
#undef PR

void write0(real *buf, long n) {
  version();
  header();
  format();
  vertices(&buf, n);
  attributes_header(n);

  int ia, na = sizeof ANAMES / sizeof ANAMES[0]; /* number of attributes */
  for (ia = 0; ia < na && ia + 3 < nrpp; ia++)
    attribute(ANAMES[ia], &buf, n);
}

void write(real *buf, long n, const char *fn) {
  if ((fo = fopen(fn, "w")) == NULL) {
    fprintf(stderr, "bop2vtk: cannot write %s\n", fn);
    die();
  }
  write0(buf, n);
  fclose(fo);
}

void endswap(real *buf, long n) {
  for (long i = 0; i < n; i++) buf[i] = endswap0(buf[i]);
}

int main(int argc, char *argv[]) {
  if (argc < 3) die();
  int iarg = 1;
  char *out = argv[iarg++];
  if (out[0] == '-' && out[1] == 'h') die(); /* help */

  long n = 0; /* number of particles */
  while (iarg < argc) n += read_bop(argv[iarg++], ibuf, &nrpp);

  in2out(ibuf, n, nrpp, obuf);
  endswap(obuf, n*nrpp);
  
  write(obuf, n, out);
  return 0;
}

/*

# TEST: bop2vtk.t0
# make bop2vtk
# ./bop2vtk o.out.vtk test_data/0008000.bop

 */
