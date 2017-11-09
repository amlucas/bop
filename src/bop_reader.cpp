#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "macros.h"
#include "utils.h"

#include "bop_reader.h"

template <typename real>
static long nvals(FILE* fd) {  /* return the number of real in the file */
    long end, curr;
    curr = ftell(fd);
    fseek(fd, 0, SEEK_END); end = ftell(fd);
    fseek(fd, curr, SEEK_SET); /* go back */
    return (end - curr) / sizeof(real);
}

template <typename real>
static long read_values(const char *fn, void **data) {
    FILE *f = fopen(fn, "r");

    if (f == NULL)
        ERR("could not open <%s>\n", fn);

    const long nreals = nvals<real>(f);
    *data = malloc(nreals * sizeof(real));
    fread(*data, sizeof(real), nreals, f); 

    fclose(f);
    return nreals;
}

#define MAXC 2048
template <typename real>
static long nreal_ascii(const char pattern[], FILE *fd) {
    fseek(fd, 0, SEEK_SET);
    long i = 0;
    char buf[MAXC] = {0};
    char *str;
    while (fscanf(fd, " %[^\n]" xstr(MAXC) "c", buf) == 1) {
        real dummy; long j = 0;
        str = buf;
        while (sscanf(str, pattern, &dummy, &j) == 1) {++i; str += j;}
        memset(buf, 0,  sizeof(buf));
    }
    fseek(fd, 0, SEEK_SET); /* go back */
    return i;
}

template <typename real>
static long read_ascii_values(const char pattern[], const char *fn, void **data) {
    FILE *f = fopen(fn, "r");
    real *d;
    if (f == NULL)
    ERR("could not open <%s>\n", fn);

    const long n = nreal_ascii<real> (pattern, f);
    
    *data = malloc(n * sizeof(real));
    d = (real*) (*data);                   
    
    char buf[MAXC] = {0}, *str;
    long i = 0, j;
    while (fscanf(f, " %[^\n]" xstr(MAXC) "c", buf) == 1) {
        real a;
        str = buf;
        j = 0;
        while (sscanf(str, pattern, &a, &j) == 1) {
            str += j;
            d[i++] = a;
        }
        memset(buf, 0,  sizeof(buf));
    }
    
    fclose(f);
    return n;
}

static void reinitc(char *buf) {memset(buf, 0, CBUFSIZE * sizeof(char));}
    
static void readline(FILE *f, char *buf) { // read full line unless it excess CBUFSIZE chars 
    reinitc(buf);
    if (fscanf(f, " %[^\n]" xstr(CBUFSIZE) "c", buf) != 1)
    ERR("line too long\n");
}

void read(const char *fnbop, BopData *d) {
    char cbuf[CBUFSIZE] = {0}, line[CBUFSIZE] = {0}, fnval[CBUFSIZE] = {0};
        
    FILE *fh = fopen(fnbop, "r");

    if (fh == NULL)
    ERR("could not open <%s>\n", fnbop);

    // parse n
    if (fscanf(fh, " %ld\n", &(d->n)) != 1)
    ERR("wrong format in <%s>\n", fnbop);

    // parse datafile name
    readline(fh, line);
        
    if (sscanf(line, "DATA_FILE: %" xstr(CBUFSIZE) "s", cbuf) != 1)
    ERR("could not read data file name\n");

    get_path(fnbop, fnval);
    strcat(fnval, cbuf);

    // parse data format
    readline(fh, line);
        
    {        
        if (sscanf(line, "DATA_FORMAT: %" xstr(CBUFSIZE) "s", cbuf) != 1)
        ERR("could not read data file format\n");

        if      (strcmp(cbuf,  "float") == 0) d->type = FLOAT;
        else if (strcmp(cbuf, "double") == 0) d->type = DOUBLE;
        else if (strcmp(cbuf,    "int") == 0) d->type = INT;
        else if (strcmp(cbuf,  "ascii") == 0) d->type = FASCII;
        else if (strcmp(cbuf, "iascii") == 0) d->type = IASCII;
        else     ERR("unknown DATA_FORMAT\n");
    }
        
    // read datafile
    switch (d->type) {
    case  FLOAT: d->nvars = read_values<float> (fnval, &d->data) / d->n; break;
    case DOUBLE: d->nvars = read_values<double>(fnval, &d->data) / d->n; break;
    case    INT: d->nvars = read_values<int>   (fnval, &d->data) / d->n; break;
    case FASCII: d->nvars = read_ascii_values<float>  (" %f%n", fnval, &d->data) / d->n; break;
    case IASCII: d->nvars = read_ascii_values<int>    (" %d%n", fnval, &d->data) / d->n; break;
    };

    // read variables
    reinitc(cbuf);
    readline(fh, line);
        
    if (sscanf(line, "VARIABLES: %[^\\0]c", cbuf) != 1)
        ERR("could not read variables entry\n");
    strcpy(d->vars, cbuf);
        
    fclose(fh);
}


