long nreal(FILE* fd) { /* return a number of reals in the file */
  long end, curr;
  curr = ftell(fd);
  fseek(fd, 0, SEEK_END); end = ftell(fd);
  fseek(fd, curr, SEEK_SET); /* go back */
  return (end - curr)/sizeof(real);
}

long read0(FILE* f, real* buf, int* nrpp) {
  /* [r]ead to [b]uffer */
  int n0; long n; /* TODO: should be long in udx */
  fread(&n0, 1, sizeof(n0), f); n = n0;
  *nrpp  = nreal(f) / n;
  fprintf(stderr, "(bop2vtk) n, nrpp: %ld %d\n", n, *nrpp);
  int sz = (*nrpp)*n;
  fread(buf, sz, sizeof(buf[0]), f);
  return n;
}

/* fn: file name; buf: input buffer; nrpp: number of reals per file */
long read(const char* fn, real* buf, int *nrpp) {
  fprintf(stderr, "(bop2vtk) reading: %s\n", fn);
  FILE* f = fopen(fn, "r");
  long n = read0(f, buf, nrpp);
  fclose(f);
  return n;
}

/* convert from
x1 y1 z1 vx1 vy1 vz1
...

to

x1 y1 z1 x2 y2 z2
...
vx1 vx2
...
vy1 vy2
...

*/
void in2out(real* ibuf, long n, int nrpp, /**/ real* obuf) {
  long i, j, dim = 3;
  for (i = 0; i < n; i++) {
    j = 0;
    for (/**/; j < dim;  j++)
      obuf[3*i + j] = ibuf[i*nrpp + j];
    for (/**/; j < nrpp; j++)
      obuf[j*n + i] = ibuf[i*nrpp + j];
  }
}
