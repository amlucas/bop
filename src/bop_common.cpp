#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "type.h"
#include "bop_common.h"
#include "macros.h"
#include "utils.h"

using namespace bop_utils;

char bop_error_msg[1024] = {0};
static const char * err_desc[_BOP_NERR] = {
    "success",
    "bad allocation",
    "bad file descriptor",
    "wrong number of variables",
    "file types mismatch"
    "wrong format"
};


BopStatus bop_alloc(BopData *d) {
    size_t sz, bsize;
    bsize = get_bsize(d->type);
    sz = d->n * d->nvars * bsize;

    return safe_malloc(sz, &d->data);
}

BopStatus bop_free(BopData *d) {
    if (d->data) free(d->data);
    // if (d->nprank) free(d->nprank);
    return BOP_SUCCESS;
}

BopStatus bop_extract_vars(const BopData *d, /**/ Cbuf *vars) {
    int i, n, ret;
    const char *v = d->vars;
    n = d->nvars;
    for (i = 0; i < n; ++i) {
        ret = sscanf(v, "%s", vars[i].c);
        if (ret != 1)
            return BOP_WRONGVAR;
        v = strstr(v, vars[i].c);
        v += strlen(vars[i].c);
    }
    return BOP_SUCCESS;
}

BopStatus bop_summary(const BopData *d) {
    fprintf(stderr, "(reader) found %ld entries, %d field(s)\n", d->n, d->nvars);
    fprintf(stderr, "\tformat: %s\n", type2str(d->type));
    fprintf(stderr, "\tvars: %s\n", d->vars);
    return BOP_SUCCESS;
}

BopStatus bop_concatenate(const int nd, const BopData *dd, BopData *dall) {
    long n          = dd[0].n;
    const BopData::Type type = dd[0].type;
    const int nvars = dd[0].nvars;
    size_t bsize;
    int i;
    long ni, start;
    const void *src;
    char *dst;
    
    for (i = 1; i < nd; ++i) {
        n += dd[i].n;
        if (type != dd[i].type || nvars != dd[i].nvars)
            return BOP_MISMATCH;
    }
    bsize = get_bsize(type);

    dall->n = n;
    dall->nvars = nvars;
    dall->type = type;
    bop_alloc(dall);

    strcpy(dall->vars, dd[0].vars);
    
    start = 0;
    
    for (i = 0; i < nd; ++i) {
        ni = dd[i].n;
        src = dd[i].data;
        dst = (char *) dall->data + bsize * start;
        memcpy(dst, src, ni * nvars * bsize);
        
        start += ni * nvars;
    }

    return BOP_SUCCESS;
}

const char * bob_report_error_desc(BopStatus status) {
    assert(status >= 0 && status < _BOP_NERR);
    return err_desc[status];
}

char * bob_report_error_mesg() {
    return bop_error_msg;
}

