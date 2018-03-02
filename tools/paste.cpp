#include <stdio.h>
#include <stdlib.h>

#include "bop_common.h"
#include "bop_serial.h"
#include "macros.h"

struct Args {
    char **in;
    char *out;
    int n;    
};

static void usg() {
    fprintf("usage: bop.paste <out> <f1.bop> <f2.bop> ... ");
    exit(1);
}

int main(int argc, char **argv) {
    BopData *in, *out;
    
    
    return 0;
}
