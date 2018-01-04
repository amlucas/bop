BopStatus bop_set_nrank(int n, BopData *d);
BopStatus bop_set_nprank(long n, BopData *d);

BopStatus bop_get_nrank(const BopData *d, int *n);
BopStatus bop_get_nprank(const BopData *d, long *n);

BopStatus bop_write_header(MPI_Comm comm, const char *name, const BopData *d);
BopStatus bop_write_values(MPI_Comm comm, const char *name, const BopData *d);

BopStatus bop_read_header(MPI_Comm comm, const char *hfname, BopData *d, char *dfname);
BopStatus bop_read_values(MPI_Comm comm, const char *dfname, BopData *d);
