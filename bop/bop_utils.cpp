#include <stdlib.h>
#include <string.h>

#include "bop_common.h"
#include "bop_utils.h"

static const char *typestr[] = {
    "float", "double", "int", "fascii", "iascii", ""};

size_t get_bsize(Type t) {
    switch(t) {
    case FLOAT:
    case FASCII:
        return sizeof(float);
    case DOUBLE:
        return sizeof(double);
    case INT:
    case IASCII:
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

Type str2type(const char *str) {
    if      (strcmp(str,  "float") == 0) return FLOAT;
    else if (strcmp(str, "double") == 0) return DOUBLE;
    else if (strcmp(str,    "int") == 0) return INT;
    else if (strcmp(str,  "ascii") == 0) return FASCII;
    else if (strcmp(str, "iascii") == 0) return IASCII;
    // default is float
    return FLOAT;
}

const char * type2str(Type t) {
    switch(t) {
    case  FLOAT: return typestr[0];
    case DOUBLE: return typestr[1];
    case    INT: return typestr[2];
    case FASCII: return typestr[3];
    case IASCII: return typestr[4];
    };
    return typestr[5];
}
