#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "bop_common.h"
#include "bop_reader.h"
#include "bop_macros.h"

static void get_path(const char *full, char *path) {
#define SEP '/'
    int i = strlen(full);
    while (--i >= 0 && full[i] != SEP);

    if (i) memcpy(path, full, (i+1)*sizeof(char));
}

template <typename real>
static long nvals(FILE* fd) {  /* return the number of real in the file */
    long end, curr;
    curr = ftell(fd);
    fseek(fd, 0, SEEK_END); end = ftell(fd);
    fseek(fd, curr, SEEK_SET); /* go back */
    return (end - curr) / sizeof(real);
}

template <typename real>
static long read_values(const char *fn, real **data) {
    FILE *f = fopen(fn, "r");

    if (f == NULL)
    ERR("could not open <%s>\n", fn);

    const long nreals = nvals<real>(f);
    *data = new real[nreals];
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
static long read_ascii_values(const char pattern[], const char *fn, real **data) {
    FILE *f = fopen(fn, "r");

    if (f == NULL)
    ERR("could not open <%s>\n", fn);

    const long n = nreal_ascii<real> (pattern, f);
    
    *data = new real[n];
    
    char buf[MAXC] = {0}, *str;
    long i = 0, j;
    while (fscanf(f, " %[^\n]" xstr(MAXC) "c", buf) == 1) {
        real a;
        str = buf;
        j = 0;
        while (sscanf(str, pattern, &a, &j) == 1) {
            str += j;
            (*data)[i++] = a;
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

static int nspaces(const char *buf) {
    int i = 0; while (buf[i] == ' ') {++i;}
    return i;
}
    
static int readword(const char *in, char *word) {
    reinitc(word);
    if (sscanf(in, " %" xstr(CBUFSIZE) "[^ ]c", word) != 1)
    ERR("could not read variable (wrong number of fields?)\n");
        
    return strlen(word) + nspaces(in);
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
    case  FLOAT: d->nvars = read_values<float> (fnval, &(d->fdata)) / d->n; break;
    case DOUBLE: d->nvars = read_values<double>(fnval, &(d->ddata)) / d->n; break;
    case    INT: d->nvars = read_values<int>   (fnval, &(d->idata)) / d->n; break;
    case FASCII: d->nvars = read_ascii_values  (" %f%n", fnval, &(d->fdata)) / d->n; break;
    case IASCII: d->nvars = read_ascii_values  (" %d%n", fnval, &(d->idata)) / d->n; break;
    };

    d->vars = new Cbuf[d->nvars];

    // read variables
    reinitc(cbuf);
    readline(fh, line);
        
    if (sscanf(line, "VARIABLES: %[^\\0]c", cbuf) != 1)
    ERR("could not read variables entry\n");

    for (int i = 0, start = 0; i < d->nvars; ++i)
    start += readword(cbuf + start, d->vars[i].c);
        
    fclose(fh);
}


