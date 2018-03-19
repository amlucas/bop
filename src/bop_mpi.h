BopStatus bop_write_header(MPI_Comm comm, const char *name, const BopData *d);
BopStatus bop_write_values(MPI_Comm comm, const char *name, const BopData *d);

BopStatus bop_read_header(MPI_Comm comm, const char *hfname, BopData *d, char *dfname);
BopStatus bop_read_values(MPI_Comm comm, const char *dfname, BopData *d);
