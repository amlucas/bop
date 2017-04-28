#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "reader.decl.h"
#include "reader.impl.h"

int main(int argc, char **argv)
{
    reader::init();
    reader::read(argv[1]);
    reader::finalize();
    
    return 0;
}
