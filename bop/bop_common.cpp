#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "bop_macros.h"
#include "bop_utils.h"

void bop_alloc(BopData *d) {
    size_t szd, szv, bsize;
    bsize = get_bsize(d->type);
    szd = d->n * d->nvars * bsize;
    szv = d->nvars * sizeof(Cbuf);
    
    d->vars = (Cbuf*) malloc(szv);
    d->data = malloc(szd);
}

void bop_free(BopData *d) {
    if (d->vars) free(d->vars);
    if (d->data) free(d->data);
}

void summary(const BopData *d) {
    fprintf(stderr, "(reader) found %ld entries, %d field(s)\n", d->n, d->nvars);
    switch(d->type) {
    case  FLOAT: fprintf(stderr, "\tformat: float\n"        ); break;
    case DOUBLE: fprintf(stderr, "\tformat: double\n"       ); break;
    case    INT: fprintf(stderr, "\tformat: int\n"          ); break;
    case FASCII: fprintf(stderr, "\tformat: ascii (float)\n"); break;
    case IASCII: fprintf(stderr, "\tformat: ascii (int)\n"  ); break;
    };
    fprintf(stderr, "\tvars:");
    for (int i = 0; i < d->nvars; ++i)
    fprintf(stderr, " %s", d->vars[i].c);
    fprintf(stderr, "\n");
}

void concatenate(const int nd, const BopData *dd, BopData *dall) {
    long n          = dd[0].n;
    const Type type = dd[0].type;
    const int nvars = dd[0].nvars;
    size_t bsize;
    int i;
    long ni, start;
    const void *src;
    char *dst;
    
    for (i = 1; i < nd; ++i) {
        n += dd[i].n;
        if (type != dd[i].type || nvars != dd[i].nvars)
        ERR("concatenate: All files must have the same format\n"); 
    }
    bsize = get_bsize(type);

    dall->n = n;
    dall->nvars = nvars;
    dall->type = type;
    bop_alloc(dall);

    for (i = 0; i < nvars; ++i)
        memcpy(dall->vars[i].c, dd[0].vars[i].c, CBUFSIZE * sizeof(char));

    
    start = 0;
    
    for (i = 0; i < nd; ++i) {
        ni = dd[i].n;
        src = dd[i].data;
        dst = (char *) dall->data + bsize * start;
        memcpy(dst, src, ni * nvars * bsize);
        
        start += ni * nvars;
    }
}
