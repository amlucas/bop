#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "reader.h"

#define xstr(s) str(s)
#define str(s) #s

#define ERR(...) do {                               \
        fprintf(stderr,"(reader) " __VA_ARGS__);    \
        exit(1);                                    \
    } while(0)

void init(ReadData *d) {
    d->n = -1;
    d->nvars = 6;
    d->vars = NULL;
    d->fdata = NULL;
    d->ddata = NULL;
    d->idata = NULL;
}

void finalize(ReadData *d) {
    if (d->vars)  delete[] d->vars;
    if (d->fdata) delete[] d->fdata;
    if (d->ddata) delete[] d->ddata;
    if (d->idata) delete[] d->idata;
}

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
    
void read(const char *fnbop, ReadData *d) {
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
        else     ERR("wrong DATA_FORMAT\n");
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

void summary(const ReadData *d) {
    fprintf(stderr, "(reader) found %ld entries, %d fields\n", d->n, d->nvars);
    switch(d->type) {
    case  FLOAT: fprintf(stderr, "\tformat: float\n" ); break;
    case DOUBLE: fprintf(stderr, "\tformat: double\n"); break;
    case    INT: fprintf(stderr, "\tformat: int\n"   ); break;
    case  ASCII: fprintf(stderr, "\tformat: ascii\n" ); break;
    };
    fprintf(stderr, "\tvars:");
    for (int i = 0; i < d->nvars; ++i)
    fprintf(stderr, " %s", d->vars[i].c);
    fprintf(stderr, "\n");
}

void concatenate(const int nd, const ReadData *dd, ReadData *dall) {
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
    case ASCII:
        ERR("ASCII: not implemented\n");
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
        else if (type == FLOAT) {
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

