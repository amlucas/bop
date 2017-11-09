#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "bop_utils.h"
#include "bop_header.h"

static int is_desc(const char *s, const char *d) {
    if (strcmp(s, d) == 0)
        return 1;
    return 0;
}

static void read_data_file(const char *data, /**/ char *df) {
    strcpy(df, data);
}

static void read_variables(const char *data, /**/ BopData *d) {
    strcpy(d->var, data);
}

static void read_type(const char *data, /**/ BopData *d) {
    d->type = str2type(data);
}

static void extract_desc_data(const char *l, /**/ char *desc, char *data) {
    const char *c;
    char *d;
    for (c = l, d = desc; l < l + strlen(l); ++c, ++d) {
        if (*c == ':')
            break;
        *d = *c;
    }
    while (*c == ':' || *c == ' ') ++c;
    strcpy(data, c);
}

static void parse_line(const char *l, /**/ char *dfname, char *var, BopData *d) {
    char desc[NC] = {0}, data[NC] = {0};
    extract_desc_data(l, /**/ desc, data);
    
    if (is_desc(desc, "DATA_FILE"))
        read_data_file(data, /**/ dfname);
    else if (is_desc(desc, "DATA_FORMAT"))
        read_type(data, /**/ d);
    else if (is_desc(desc, "VARIABLES"))
        return;
    else
        printf("unprocessed desc: <%s>, data: <%s>\n", desc, data);
}

static void read_n_data(const char *data, /**/ BopData *d) {
    long n;
    sscanf(data, "%ld", &n);
    d->n = n;
}


void bop_read_header(const char *fname, /**/ char *dfname, BopData *d) {
    FILE *f;
    char line[NC], var[NC];
    int l = 0;

    f = fopen(fname, "r");
    
    while (EOF != fscanf(f, " %" xstr(NC) "[^\n]c", line)) {
        if (l == 0) read_n_data(line, /**/ d);
        else        parse_line(line, /**/ dfname, var, d);
        ++l;
    }    
    
    fclose(f);
}

void bop_write_header(const char *fhname, const char *fdname, const BopData *d);
