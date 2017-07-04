#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "bop_common.h"
#include "bop_macros.h"

void init(BopData *d) {
    d->n = -1;
    d->nvars = 6;
    d->vars = NULL;
    d->fdata = NULL;
    d->ddata = NULL;
    d->idata = NULL;
}

void finalize(BopData *d) {
    if (d->vars)  delete[] d->vars;
    if (d->fdata) delete[] d->fdata;
    if (d->ddata) delete[] d->ddata;
    if (d->idata) delete[] d->idata;
}

void summary(const BopData *d) {
    fprintf(stderr, "(reader) found %ld entries, %d field(s)\n", d->n, d->nvars);
    switch(d->type) {
    case  FLOAT: fprintf(stderr, "\tformat: float\n" ); break;
    case DOUBLE: fprintf(stderr, "\tformat: double\n"); break;
    case    INT: fprintf(stderr, "\tformat: int\n"   ); break;
    case FASCII: fprintf(stderr, "\tformat: ascii\n" ); break;
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
    case FASCII:
        dall->fdata = new float[n*nvars];
        break;
    case FLOAT:
        dall->fdata = new float[n*nvars];
        break;
    case DOUBLE:
        dall->ddata = new double[n*nvars];
        break;
    case INT:
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
            memcpy(dst, src, ni * nvars * sizeof(float));
        }
        else if (type == FLOAT || type == FASCII) {
            const float *src = dd[i].fdata;
            float *dst = dall->fdata + start;
            memcpy(dst, src, ni * nvars * sizeof(float));
        }
        else if (type == INT) {
            const int *src = dd[i].idata;
            int *dst = dall->idata + start;
            memcpy(dst, src, ni * nvars * sizeof(int));
        }
        
        start += ni * nvars;
    }
}
