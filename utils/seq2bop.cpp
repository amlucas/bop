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

    BopData d;
    init(&d);

    const int N = atoi(argv[1]);

    d.n = N;
    d.idata = new int[N];
    for (int i = 0; i < N; ++i) d.idata[i] = i;
    d.type = IASCII;
    d.nvars = 1;
    d.vars = new Cbuf[d.nvars];
    strncpy(d.vars[0].c, "seq", 4);

    write(argv[2], d);
    
    finalize(&d);
    return 0;
}
