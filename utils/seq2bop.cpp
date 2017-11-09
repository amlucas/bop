#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "bop_common.h"
#include "bop_writer.h"

int main(int argc, char **argv) {

    if (argc != 3) {
        fprintf(stderr, "usage: ./seq2bop <N> <out.bop>\n");
        exit(1);
    }
    
    int *data, i;
    BopData d;

    const int N = atoi(argv[1]);

    d.n = N;
    d.type = IASCII;
    d.nvars = 1;

    bop_alloc(&d);
    data = (int *) d.data;
    
    for (i = 0; i < N; ++i) data[i] = i;

    strcpy(d.vars, "seq");

    write(argv[2], d);
    
    bop_free(&d);
    return 0;
}
