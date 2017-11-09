#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "header.h"
#include "macros.h"
#include "utils.h"

#include "bop_serial.h"

void bop_write_header(const char *name, const BopData *d) {
    using namespace bop_header;
    using namespace bop_utils;
    
    char fnval[CBUFSIZE] = {0},
        fnval0[CBUFSIZE] = {0},
        fnhead[CBUFSIZE] = {0};

    sprintf(fnhead, "%s.bop", name);

    get_path(fnhead, fnval);
    get_fname_values(fnhead, fnval0);
    strcat(fnval, fnval0);

    write_header(name, fnval0, d);
}

template <typename T>
static void write_ascii(const char pattern[], const T *data, const long n, const long nvars, FILE *f) {
    long i, k, j = 0;
    for (i = 0; i < n; ++i) {
        for (k = 0; k < nvars; ++k)
            fprintf(f, pattern, data[j++]);
        fprintf(f, "\n");
    }
}

static void write_data(const char *fnval, const BopData *d) {
    using namespace bop_utils;
    FILE *fd;
    size_t bsize;

    safe_open(fnval, "w", &fd);
    bsize = get_bsize(d->type);

    switch(d->type) {
    case FLOAT:
    case DOUBLE:
    case INT:
        fwrite(d->data, bsize, d->n * d->nvars, fd);
        break;
    case FASCII:
        write_ascii("%.6e ", (const float*) d->data, d->n, d->nvars, fd);
        break;
    case IASCII:
        write_ascii("%d ", (const int*) d->data, d->n, d->nvars, fd);
        break;
    }
    fclose(fd);    
}


void bop_write_values(const char *name, const BopData *d) {
    char dfname[CBUFSIZE] = {0};
    sprintf(dfname, "%s.values", name);
    write_data(dfname, d);
}

void bop_read_header(const char *hfname, BopData *d, char *dfname) {
    using namespace bop_header;
    using namespace bop_utils;
    
    char dfname0[CBUFSIZE] = {0}, locdfname[CBUFSIZE] = {0};
    read_header(hfname, /**/ dfname0, d);

    get_path(hfname, locdfname);
    strcat(locdfname, dfname0);
    strcpy(dfname, locdfname);
}

static void read_values(const char *fn, long n, int nvars, size_t bsize, void *data) {
    FILE *f = fopen(fn, "r");

    if (f == NULL)
        ERR("could not open <%s>\n", fn);

    fread(data, bsize, n * nvars, f); 

    fclose(f);
}

template <typename real>
static void read_ascii_values(const char pattern[], const char *fn, void *data) {
    using namespace bop_utils;
    
    char buf[CBUFSIZE] = {0}, *str;
    long i = 0, j;
    FILE *f;
    real *d, a;

    safe_open(fn, "r", &f);
    
    d = (real*) data;        
    
    while (fscanf(f, " %[^\n]" xstr(MAXC) "c", buf) == 1) {
        str = buf;
        j = 0;
        while (sscanf(str, pattern, &a, &j) == 1) {
            str += j;
            d[i++] = a;
        }
        memset(buf, 0,  sizeof(buf));
    }
    
    fclose(f);
}


void bop_read_values(const char *dfname, BopData *d) {
    using namespace bop_utils;
    
    size_t bsize;
    bsize = get_bsize(d->type);

    switch (d->type) {
    case  FLOAT: 
    case DOUBLE: 
    case    INT:
        read_values(dfname, d->n, d->nvars, bsize, d->data);
        break;
    case FASCII:
        read_ascii_values<float>(" %f%n", dfname, d->data);
        break;
    case IASCII:
        read_ascii_values<int>  (" %d%n", dfname, d->data);
        break;
    };
}
