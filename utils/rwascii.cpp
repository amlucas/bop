#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "bop_common.h"
#include "bop_reader.h"
#include "bop_writer.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: ./%s <in.bop> <out.bop>\n", argv[0]);
        exit(1);
    }

    BopData d;
    read(argv[1], &d);
    summary(&d);
    write(argv[2], d);    
    bop_free(&d);
    
    return 0;
}

/*

  # nTEST: ascii2ascii.t0
  # make -j 
  # ./rwascii data/ascii.bop test.bop
  # mv test.values test.out.txt

  # nTEST: ascii2ascii.t1
  # make -j 
  # ./rwascii data/iascii.bop test.bop
  # mv test.values test.out.txt

*/
