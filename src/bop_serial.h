void bop_write_header(const char *name, const BopData *d);
void bop_write_data(const char *name, const BopData *d);

void bop_read_header(const char *hfname, BopData *d, char *dfname);
void bop_read_data(const char *dfname, BopData *d);
