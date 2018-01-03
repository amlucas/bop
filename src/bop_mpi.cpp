#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bop_common.h"
#include "header.h"
#include "macros.h"
#include "utils.h"

#include "bop_mpi.h"

BopStatus bop_write_header(const char *name, const BopData *d) {}
BopStatus bop_write_values(const char *name, const BopData *d) {}

BopStatus bop_read_header(const char *hfname, BopData *d, char *dfname) {}
BopStatus bop_read_values(const char *dfname, BopData *d) {}
