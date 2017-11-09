#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "bop_utils.h"
#include "bop_macros.h"
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
    strcpy(d->vars, data);
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

static void parse_line(const char *l, /**/ char *dfname, BopData *d) {
    char desc[CBUFSIZE] = {0}, data[CBUFSIZE] = {0};
    extract_desc_data(l, /**/ desc, data);
    
    if (is_desc(desc, "DATA_FILE"))
        read_data_file(data, /**/ dfname);
    else if (is_desc(desc, "DATA_FORMAT"))
        read_type(data, /**/ d);
    else if (is_desc(desc, "VARIABLES"))
        read_variables(data, /**/ d);
    else
        printf("unprocessed desc: <%s>, data: <%s>\n", desc, data);
}

static void read_n_data(const char *data, /**/ BopData *d) {
    long n;
    sscanf(data, "%ld", &n);
    d->n = n;
}

static int count_vars(const char *var) {
    int i;
    char buf[CBUFSIZE];

    i = 0;
    while (1 == sscanf(var, "%s", buf)) {
        var = strstr(var, buf);
        var += strlen(buf);
        ++i;
    }
    return i;
}

void bop_read_header(const char *fname, /**/ char *dfname, BopData *d) {
    FILE *f;
    char line[CBUFSIZE];
    int l = 0;

    f = fopen(fname, "r");
    
    while (EOF != fscanf(f, " %" xstr(CBUFSIZE) "[^\n]c", line)) {
        if (l == 0) read_n_data(line, /**/ d);
        else        parse_line(line, /**/ dfname, d);
        ++l;
    }    

    d->nvars = count_vars(d->vars);
    
    fclose(f);
}



void bop_write_header(const char *fhname, const char *fdname, const BopData *d) {
    FILE *f = fopen(fhname, "w");
    
    fprintf(f, "%ld\n", d->n);
    fprintf(f, "DATA_FILE: %s\n", fdname);
    fprintf(f, "DATA_FORMAT: %s\n", type2str(d->type));
    fprintf(f, "VARIABLES %s", d->vars);
    fprintf(f, "\n");
    
    fclose(f);
}
