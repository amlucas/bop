#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "type.h"
#include "header.h"
#include "macros.h"
#include "utils.h"

#include "bop_mpi.h"

BopStatus bop_set_nrank(int n, BopData *d);
BopStatus bop_set_nprank(long n, BopData *d);

BopStatus bop_get_nrank(const BopData *d, int *n);
BopStatus bop_get_nprank(const BopData *d, long *n);


BopStatus bop_write_header(MPI_Comm comm, const char *name, const BopData *d) {}
BopStatus bop_write_values(MPI_Comm comm, const char *name, const BopData *d) {}

BopStatus bop_read_header(MPI_Comm comm, const char *hfname, BopData *d, char *dfname) {
    using namespace bop_header;
    using namespace bop_utils;
    BopStatus s;
    char dfname0[CBUFSIZE] = {0}, locdfname[CBUFSIZE] = {0};
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    
    s = read_header(rank, hfname, /**/ dfname0, d);

    if (d->nrank != size)
        return BOP_WMPISIZE;
    
    get_path(hfname, locdfname);
    strcat(locdfname, dfname0);
    strcpy(dfname, locdfname);

    return s;
}

BopStatus bop_read_values(MPI_Comm comm, const char *dfname, BopData *d) {}
