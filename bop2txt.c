#include <stdio.h>

typedef float real;
#include "bop.utils.h"
#define fmt "%-.16e"

#define MAX_PART_NUM 1000000
#define MAX_LINE     15     /* max number of fields per particle */
real buf[MAX_LINE*MAX_PART_NUM]; /* input and output buffers */

void write(real* buf, long n, int nrpp) {
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < nrpp; j++) {
      if (j > 0) putchar(' ');
      printf(fmt, *buf);
      buf++;
    }
    putchar('\n');
  }
}

int main(int argc, char *argv[]) {
  int nrpp; /* [n]umber of [r]eals [p]er [p]article */
  long n = 0; /* number of particles */
  for (/**/; --argc; ++argv)  n += read(argv[1], buf, &nrpp);
  write(buf, n, nrpp);
  return 0;
}
