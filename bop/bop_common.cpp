#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "bop_macros.h"
#include "bop_utils.h"

void init(BopData *d) {
    d->n = -1;
    d->nvars = 6;
    d->vars = NULL;
    d->data = NULL;
}

void finalize(BopData *d) {
    if (d->vars) free(d->vars);
    if (d->data) free(d->fdata);
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
    
    for (int i = 1; i < nd; ++i) {
        n += dd[i].n;
        if (type != dd[i].type || nvars != dd[i].nvars)
        ERR("concatenate: All files must have the same format\n"); 
    }

    dall->vars = new Cbuf[nvars];
    for (int i = 0; i < nvars; ++i)
        memcpy(dall->vars[i].c, dd[0].vars[i].c, CBUFSIZE * sizeof(char));

    
    switch (type) {
    case FLOAT:
    case FASCII:
        dall->fdata = new float[n*nvars];
        break;
    case DOUBLE:
        dall->ddata = new double[n*nvars];
        break;
    case INT:
    case IASCII:
        dall->idata = new int[n*nvars];
        break;
    };

    dall->n = n;
    dall->nvars = nvars;
    dall->type = type;
    
    long start = 0;
    
    for (int i = 0; i < nd; ++i) {
        const long ni = dd[i].n;
        
        if (type == DOUBLE) {
            const double *src = dd[i].ddata;
            double *dst = dall->ddata + start;
            memcpy(dst, src, ni * nvars * sizeof(double));
        }
        else if (type == FLOAT || type == FASCII) {
            const float *src = dd[i].fdata;
            float *dst = dall->fdata + start;
            memcpy(dst, src, ni * nvars * sizeof(float));
        }
        else if (type == INT || type == IASCII) {
            const int *src = dd[i].idata;
            int *dst = dall->idata + start;
            memcpy(dst, src, ni * nvars * sizeof(int));
        }
        
        start += ni * nvars;
    }
}
