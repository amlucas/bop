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
    fprintf(stderr, "usage: bop.paste <out> <f1.bop> <f2.bop> ... ");
    exit(1);
}

static int shift_args(int *c, char ***v) {
    (*c)--;
    (*v)++;
    return (*c) > 0;
}

static void parse(int argc, char **argv, Args *a) {
    if (!shift_args(&argc, &argv)) usg();
    a->out = *argv;

    if (!shift_args(&argc, &argv)) usg();
    a->in = argv;
    a->n = argc;
}

int main(int argc, char **argv) {
    BopData *in, *out;
    Args a;
    parse(argc, argv, &a);
    
    
    
    return 0;
}
