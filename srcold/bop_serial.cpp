#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "header.h"
#include "macros.h"
#include "utils.h"

#include "bop_serial.h"

using namespace bop_header;
using namespace bop_utils;

BopStatus bop_write_header(const char *name, const BopData *d) {    
    char fnval[CBUFSIZE] = {0},
        fnval0[CBUFSIZE] = {0},
        fnhead[CBUFSIZE] = {0};

    sprintf(fnhead, "%s.bop", name);

    get_path(fnhead, fnval);
    get_fname_values(fnhead, fnval0);
    strcat(fnval, fnval0);

    return write_header(name, fnval0, d);
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

static BopStatus write_data(const char *fnval, const BopData *d) {
    FILE *fd;
    size_t bsize;
    BopStatus s;

    s = safe_open(fnval, "w", &fd);
    bsize = get_bsize(d->type);

    if (s != BOP_SUCCESS) return s;

    switch(d->type) {
    case BopData::FLOAT:
    case BopData::DOUBLE:
    case BopData::INT:
        fwrite(d->data, bsize, d->n * d->nvars, fd);
        break;
    case BopData::FASCII:
        write_ascii("%.6e ", (const float*) d->data, d->n, d->nvars, fd);
        break;
    case BopData::IASCII:
        write_ascii("%d ", (const int*) d->data, d->n, d->nvars, fd);
        break;
    }
    fclose(fd);

    return s;
}


BopStatus bop_write_values(const char *name, const BopData *d) {
    char dfname[CBUFSIZE] = {0};
    sprintf(dfname, "%s.values", name);
    return write_data(dfname, d);
}

BopStatus bop_read_header(const char *hfname, BopData *d, char *dfname) {
    using namespace bop_header;
    using namespace bop_utils;
    BopStatus s;
    char dfname0[CBUFSIZE] = {0}, locdfname[CBUFSIZE] = {0};
    s = read_header(hfname, /**/ dfname0, d);

    get_path(hfname, locdfname);
    strcat(locdfname, dfname0);
    strcpy(dfname, locdfname);

    return s;
}

static BopStatus read_values(const char *fn, long n, int nvars, size_t bsize, void *data) {
    FILE *f;
    BopStatus s;
    s = safe_open(fn, "r", &f);
    if (s != BOP_SUCCESS) return s;

    fread(data, bsize, n * nvars, f); 

    fclose(f);
    return s;
}

template <typename real>
static BopStatus read_ascii_values(const char pattern[], const char *fn, void *data) {
    char buf[CBUFSIZE] = {0}, *str;
    long i = 0, j;
    FILE *f;
    BopStatus s;
    real *d, a;

    s = safe_open(fn, "r", &f);
    if (s != BOP_SUCCESS) return s;
    
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
    return s;
}


BopStatus bop_read_values(const char *dfname, BopData *d) {
    size_t bsize;
    bsize = get_bsize(d->type);

    switch (d->type) {
    case BopData::FLOAT: 
    case BopData::DOUBLE: 
    case BopData::INT:
        return read_values(dfname, d->n, d->nvars, bsize, d->data);
    case BopData::FASCII:
        return read_ascii_values<float>(" %f%n", dfname, d->data);
    case BopData::IASCII:
        return read_ascii_values<int>  (" %d%n", dfname, d->data);
    };
    return BOP_SUCCESS;
}