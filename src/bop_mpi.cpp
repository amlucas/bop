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

using namespace bop_header;
using namespace bop_utils;

BopStatus bop_set_nrank(int n, BopData *d)  {
    if (d) {
        d->nrank = n;
        return BOP_SUCCESS;
    }
    return BOP_NULLPTR;
}

BopStatus bop_set_nprank(long n, BopData *d)  {
    if (d) {
        d->nprank = n;
        return BOP_SUCCESS;
    }
    return BOP_NULLPTR;
}

BopStatus bop_get_nrank(const BopData *d, int *n)  {
    if (d) {
        *n = d->nrank;
        return BOP_SUCCESS;
    }
    return BOP_NULLPTR;
}
BopStatus bop_get_nprank(const BopData *d, long *n)  {
    if (d) {
        *n = d->nprank;
        return BOP_SUCCESS;
    }
    return BOP_NULLPTR;
}

template <typename T>
static void write_mpi(MPI_Comm comm, const char *fname, long n, const T *data, MPI_Datatype type) {
    MPI_Offset base, offset, len;
    MPI_Status status;
    MPI_File f;

    MPI_File_open(comm, fname , MPI_MODE_WRONLY | MPI_MODE_CREATE, MPI_INFO_NULL, &f);
    MPI_File_set_size(f, 0);
    MPI_File_get_position(f, &base);

    len = n * sizeof(T);
    offset = 0;
    MPI_Exscan(&len, &offset, 1, MPI_OFFSET, MPI_SUM, comm);
    MPI_File_write_at_all(f, base + offset, data, n, type, &status);
    MPI_File_close(&f);
}

static BopStatus write_header(MPI_Comm comm, const char *fhname, const char *fdname, const BopData *d) {
    int rank, n;
    char *buf;
    BopStatus s;
    enum {MAX_SIZE = 2048};
    MPI_Comm_rank(comm, &rank);

    s = safe_malloc(MAX_SIZE * sizeof(char), (void**) &buf);
    if (s != BOP_SUCCESS) return s;

    n = 0;
    if (rank == 0) {
        n += sprintf(buf + n, "%ld\n", d->n);
        n += sprintf(buf + n, "DATA_FILE: %s\n", fdname);
        n += sprintf(buf + n, "DATA_FORMAT: %s\n", type2str(d->type));
        n += sprintf(buf + n, "VARIABLES %s\n", d->vars);
        n += sprintf(buf + n, "NRANK %ld\n", d->nrank);
    }
    n += sprintf(buf + n, "%ld\n", d->nprank);

    write_mpi(comm, fhname, n, buf, MPI_CHAR);
    
    free(buf);    
    return s;
}

BopStatus bop_write_header(MPI_Comm comm, const char *name, const BopData *d) {
    char fnval[CBUFSIZE] = {0},
        fnval0[CBUFSIZE] = {0},
        fnhead[CBUFSIZE] = {0};

    sprintf(fnhead, "%s.bop", name);

    get_path(fnhead, fnval);
    get_fname_values(fnhead, fnval0);
    strcat(fnval, fnval0);

    return write_header(comm, name, fnval0, d);
}

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
