#include <stdio.h>

typedef float real;
#include "bop.utils.h"
#define fmt "%.15e"

#define MAX_PART_NUM 1000000
#define MAX_LINE     15     /* max number of fields per particle */
real buf[MAX_LINE*MAX_PART_NUM]; /* input and output buffers */

void write0(real *buf, long n, int nrpp, FILE *f) {
  int n0 = n;
  printf("%d\n", n0);
  fwrite(&n0, sizeof(n0), 1, f);
  fwrite(buf, sizeof(buf[0]), n*nrpp, f);
}

void write(real *buf, long n, int nrpp, char *fn) {
  FILE *f = fopen(fn, "w");
  write0(buf, n, nrpp, f);
  fclose(f);
}

int  ws(char c) { return c == ' ' || c == '\t'; } /*       [w]hite [s]pace */
int nws(char c) { return                !ws(c); } /* [n]ot [w]hite [s]pace */
int nf() { /* number of fields */
  int i, cnt;
  char s[BUFSIZ];
  if (fgets(s, BUFSIZ, stdin) == NULL) return 0;
  rewind(stdin);
  
  for (i = cnt = 0; s[i] != '\0'; i++) {
    if (ws(s[i])) continue;
    if (i - 1 < 0 || ws(s[i - 1])) cnt++;
  }
  return cnt;
} 
  
long read(real *buf, int *nrpp) {
  int cnt = 0;
  *nrpp = nf(); /* number of fields */
  while (scanf("%f", &buf[cnt]) == 1) cnt++;
  return cnt/(*nrpp);
}

int main(int argc, char *argv[]) {
  int nrpp;
  long n = read(buf, &nrpp);
  
  char *fn = argv[1];
  write(buf, n, nrpp, fn);
  return 0;
}
