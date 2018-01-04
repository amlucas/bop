#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "utils.h"
#include "macros.h"

namespace bop_utils {

static const char *typestr[] = {
    "float", "double", "int", "fascii", "iascii", ""};

BopStatus safe_malloc(size_t sz, void **data) {
    *data = malloc(sz);
    
    if (*data == NULL) {
        sprintf(bop_error_msg,
                ":%s:%d: could not allocate array of %ld bytes\n",
                __FILE__, __LINE__, sz);
        return BOP_BADALLOC;
    }
    return BOP_SUCCESS;
}

BopStatus safe_open(const char *fname, const char *mode, FILE **f) {
    *f = fopen(fname, mode);
    if (*f == NULL) {
        sprintf(bop_error_msg,
                ":%s:%d: could not open <%s>\n",
                __FILE__, __LINE__, fname);
        return BOP_BADFILE;
    }
    return BOP_SUCCESS;
}

size_t get_bsize(BopData::Type t) {
    switch(t) {
    case BopData::FLOAT:
    case BopData::FASCII:
        return sizeof(float);
    case BopData::DOUBLE:
        return sizeof(double);
    case BopData::INT:
    case BopData::IASCII:
        return sizeof(int);
    };
    return 0;
}

#define SEP '/'
void get_path(const char *full, char *path) {
    int i = strlen(full);
    while (--i >= 0 && full[i] != SEP);

    if (i) memcpy(path, full, (i+1)*sizeof(char));
}

void get_fname_values(const char *fnbop, char *fnval) {
    int i = strlen(fnbop);
    const int n = i;
    while (--i >= 0 && fnbop[i] != SEP);

    memcpy(fnval, fnbop + i + 1, (n-i)*sizeof(char));

    i = strlen(fnval);
    strncpy(fnval + i - 4, ".values", 8);
}

#undef SEP

BopData::Type str2type(const char *str) {
    if      (strcmp(str,  "float") == 0) return BopData::FLOAT;
    else if (strcmp(str, "double") == 0) return BopData::DOUBLE;
    else if (strcmp(str,    "int") == 0) return BopData::INT;
    else if (strcmp(str,  "ascii") == 0) return BopData::FASCII;
    else if (strcmp(str, "iascii") == 0) return BopData::IASCII;
    // default is float
    return BopData::FLOAT;
}

const char * type2str(BopData::Type t) {
    switch(t) {
    case BopData::FLOAT:  return typestr[0];
    case BopData::DOUBLE: return typestr[1];
    case BopData::INT:    return typestr[2];
    case BopData::FASCII: return typestr[3];
    case BopData::IASCII: return typestr[4];
    };
    return typestr[5];
}

} // bop_header
