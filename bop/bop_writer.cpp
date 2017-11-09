#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "bop_writer.h"
#include "bop_macros.h"
#include "bop_utils.h"

static void type2str(const Type type, /**/ char *str) {
#define assign(arg) strncpy(str, arg, CBUFSIZE)
    switch(type) {
    case FLOAT:  assign("float");  break;
    case FASCII: assign("ascii");  break;
    case DOUBLE: assign("double"); break;
    case INT:    assign("int");    break;
    case IASCII: assign("iascii"); break;
    }
#undef assign
}

static void header(const char *fnbop, const char *fnval, const BopData d) {
    FILE *fh = fopen(fnbop, "w");
    if (fh == NULL) ERR("could not open <%s>\n", fnbop);    

    char format[CBUFSIZE] = {0};
    type2str(d.type, format);
    
    fprintf(fh, "%ld\n", d.n);
    fprintf(fh, "DATA_FILE: %s\n", fnval);
    fprintf(fh, "DATA_FORMAT: %s\n", format);
    fprintf(fh, "VARIABLES:");
    for (int i = 0; i < d.nvars; ++i) fprintf(fh, " %s", d.vars[i].c);
    fprintf(fh, "\n");                
    fclose(fh);    
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

static void data(const char *fnval, const BopData d) {
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

void write(const char *fnbop, const BopData d) {
    char fnval[CBUFSIZE] = {0}, fnval0[CBUFSIZE] = {0};

    get_path(fnbop, fnval);
    get_fname_values(fnbop, fnval0);
    strcat(fnval, fnval0);

    header(fnbop, fnval0, d);
    data(fnval, d);
}


