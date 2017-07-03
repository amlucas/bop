#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "bop_writer.h"

int main(int argc, char **argv) {

    BopData d;
    init(&d);

    const int N = 10;

    d.n = N;
    d.idata = new int[N];
    for (int i = 0; i < N; ++i) d.idata[i] = i;
    d.type = INT;
    d.nvars = 1;
    d.vars = new Cbuf[1];
    strncpy(d.vars[0].c, "seq", 4);

    write("test.bop", d);
    
    finalize(&d);
    return 0;
}
