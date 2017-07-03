#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "bop_reader.h"
#include "bop_macros.h"

namespace {
void get_path(const char *full, char *path) {
#define SEP '/'
    int i = strlen(full);
    while (--i >= 0 && full[i] != SEP);

    if (i) memcpy(path, full, (i+1)*sizeof(char));
}

template<typename real>
long nvals(FILE* fd) {  /* return the number of real in the file */
    long end, curr;
    curr = ftell(fd);
    fseek(fd, 0, SEEK_END); end = ftell(fd);
    fseek(fd, curr, SEEK_SET); /* go back */
    return (end - curr) / sizeof(real);
}

template<typename real>
long read_values(const char *fn, real **data) {
    FILE *f = fopen(fn, "r");

    if (f == NULL)
    ERR("could not open <%s>\n", fn);

    const long nreals = nvals<real>(f);
    *data = new real[nreals];
    fread(*data, sizeof(real), nreals, f); 

    fclose(f);
    return nreals;
}

void reinitc(char *buf) {memset(buf, 0, CBUFSIZE * sizeof(char));}
    
void readline(FILE *f, char *buf) { // read full line unless it excess CBUFSIZE chars 
    reinitc(buf);
    if (fscanf(f, " %[^\n]" xstr(CBUFSIZE) "c", buf) != 1)
    ERR("line too long\n");
}

int nspaces(const char *buf) {
    int i = 0; while (buf[i] == ' ') {++i;}
    return i;
}
    
int readword(const char *in, char *word) {
    reinitc(word);
    if (sscanf(in, " %" xstr(CBUFSIZE) "[^ ]c", word) != 1)
    ERR("could not read variable (wrong number of fields?)\n");
        
    return strlen(word) + nspaces(in);
}
} // anonymous namespace

void read(const char *fnbop, BopData *d) {
    char cbuf[CBUFSIZE] = {0}, line[CBUFSIZE] = {0}, fnval[CBUFSIZE] = {0};
        
    FILE *fh = fopen(fnbop, "r");

    if (fh == NULL)
    ERR("could not open <%s>\n", fnbop);

    // parse n
    if (fscanf(fh, " %ld\n", &(d->n)) != 1)
    ERR("wrong format\n");

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
        else if (strcmp(cbuf,  "ascii") == 0) d->type = ASCII;
        else     ERR("unknown DATA_FORMAT\n");
    }
        
    // read datafile
    switch (d->type) {
    case  FLOAT: d->nvars = read_values<float> (fnval, &(d->fdata)) / d->n; break;
    case DOUBLE: d->nvars = read_values<double>(fnval, &(d->ddata)) / d->n; break;
    case    INT: d->nvars = read_values<int>   (fnval, &(d->idata)) / d->n; break;
    case  ASCII: ERR("ASCII: not implemented\n");
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


