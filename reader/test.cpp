#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "reader.decl.h"
#include "reader.impl.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <file.bop>\n", argv[0]);
        exit(1);
    }
    
    reader::init();
    reader::read(argv[1]);
    reader::summary();
    reader::finalize();
    
    return 0;
}
