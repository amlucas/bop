#include <stdlib.h>

#include "bop_common.h"
#include "bop_utils.h"

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
