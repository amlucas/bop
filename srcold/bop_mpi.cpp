#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "header.h"
#include "macros.h"
#include "utils.h"

#include "bop_mpi.h"

BopStatus bop_write_header(MPI_Comm comm, const char *name, const BopData *d) {}
BopStatus bop_write_values(MPI_Comm comm, const char *name, const BopData *d) {}

BopStatus bop_read_header(MPI_Comm comm, const char *hfname, BopData *d, char *dfname) {}
BopStatus bop_read_values(MPI_Comm comm, const char *dfname, BopData *d) {}
