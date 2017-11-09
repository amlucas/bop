#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "bop_macros.h"
#include "bop_utils.h"

void bop_alloc(BopData *d) {
    size_t sz, bsize;
    bsize = get_bsize(d->type);
    sz = d->n * d->nvars * bsize;    

    d->data = malloc(sz);
}

void bop_free(BopData *d) {
    if (d->data) free(d->data);
}

void bop_extract_vars(const BopData *d, /**/ Cbuf *vars) {
    int i, n;
    const char *v = d->vars;
    n = d->nvars;
    for (i = 0; i < n; ++i) {
        sscanf(v, "%s", vars[i].c);
        v = strstr(v, vars[i].c);
        v += strlen(vars[i].c);
    }
}

void summary(const BopData *d) {
    fprintf(stderr, "(reader) found %ld entries, %d field(s)\n", d->n, d->nvars);
    fprintf(stderr, "\tformat: %s\n", type2str(d->type));
    fprintf(stderr, "\tvars: %s\n", d->vars);
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

    strcpy(dall->vars, dd[0].vars);
    
    start = 0;
    
    for (i = 0; i < nd; ++i) {
        ni = dd[i].n;
        src = dd[i].data;
        dst = (char *) dall->data + bsize * start;
        memcpy(dst, src, ni * nvars * bsize);
        
        start += ni * nvars;
    }
}
