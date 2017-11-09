#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "bop_header.h"
#include "bop_macros.h"
#include "bop_utils.h"

#include "bop_serial.h"

void write_header(const char *name, const BopData *d) {
    char fnval[CBUFSIZE] = {0},
        fnval0[CBUFSIZE] = {0},
        fnhead[CBUFSIZE] = {0};

    sprintf(fnhead, "%s.bop", name);

    get_path(fnhead, fnval);
    get_fname_values(fnhead, fnval0);
    strcat(fnval, fnval0);

    bop_write_header(name, fnval0, d);
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

static void write_data(const char *fnval, const BopData d) {
    FILE *fd = fopen(fnval, "w");
    size_t bsize;
    if (fd == NULL)
        ERR("could not open <%s>\n", fnval);

    bsize = get_bsize(d.type);

    const int N = d.n * d.nvars;
    
    switch(d.type) {
    case FLOAT:
    case DOUBLE:
    case INT:
        fwrite(d.data, bsize, N, fd);
        break;
    case FASCII:
        write_ascii("%.6e ", (const float*) d.data, d.n, d.nvars, fd);
        break;
    case IASCII:
        write_ascii("%d ", (const int*) d.data, d.n, d.nvars, fd);
        break;
    }
    fclose(fd);    
}


void write_data(const char *name, const BopData *d) {
    char dname[CBUFSIZE];
    sprintf(dname, "%s.values", name);
    write_data(dfname, d);
}

void read_header(const char *hfname, BopData *d, char *dfname);
void read_data(const char *dfname, BopData *d);
