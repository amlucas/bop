#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "bop_reader.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./%s <in.bop>\n", argv[0]);
        exit(1);
    }

    BopData d;
    init(&d);
    read(argv[1], &d);
    summary(&d);
    finalize(&d);
    
    return 0;
}

