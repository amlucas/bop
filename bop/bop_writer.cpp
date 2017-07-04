#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "bop_writer.h"
#include "bop_macros.h"

namespace {
#define SEP '/'
void get_path(const char *full, char *path) {
    int i = strlen(full);
    while (--i >= 0 && full[i] != SEP);

    if (i) memcpy(path, full, (i+1)*sizeof(char));
}

void get_fnval0(const char *fnbop, char *fnval0) {
    int i = strlen(fnbop);
    const int n = i;
    while (--i >= 0 && fnbop[i] != SEP);

    memcpy(fnval0, fnbop + i + 1, (n-i)*sizeof(char));

    i = strlen(fnval0);
    strncpy(fnval0 + i - 4, ".values", 8);
}
#undef SEP

void type2str(const Type type, /**/ char *str) {
#define assign(arg) strncpy(str, arg, CBUFSIZE)
    switch(type) {
    case FLOAT:  assign("float");  break;
    case ASCII:  assign("ascii");  break;
    case DOUBLE: assign("double"); break;
    case INT:    assign("int");    break;
    }
#undef assign
}

void header(const char *fnbop, const char *fnval, const BopData d) {
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

void write_ascii(const float *data, const long n, const long nvars, FILE *f) {
    long j = 0;
    for (long i = 0; i < n; ++i) {
        for (long k = 0; k < nvars; ++k)
        fprintf(f, "%.6e ", data[j++]);
        fprintf(f, "\n");
    }
}

void data(const char *fnval, BopData d) {
    FILE *fd = fopen(fnval, "w");
    if (fd == NULL) ERR("could not open <%s>\n", fnval);

    const int N = d.n * d.nvars;
    
    switch(d.type) {
    case FLOAT:
        fwrite(d.fdata, sizeof(float), N, fd);
        break;
    case ASCII:
        write_ascii(d.fdata, d.n, d.nvars, fd);
        break;
    case DOUBLE:
        fwrite(d.ddata, sizeof(double), N, fd);
        break;        
    case INT:
        fwrite(d.idata, sizeof(int), N, fd);
        break;
    }
    fclose(fd);    
}
} // anonymous namespace

void write(const char *fnbop, const BopData d) {
    char fnval[CBUFSIZE] = {0}, fnval0[CBUFSIZE] = {0};

    get_path(fnbop, fnval);
    get_fnval0(fnbop, fnval0);
    strcat(fnval, fnval0);

    header(fnbop, fnval0, d);
    data(fnval, d);
}


