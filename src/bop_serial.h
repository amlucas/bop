void write_header(const char *name, const BopData *d);
void write_data(const char *name, const BopData *d);

void read_header(const char *hfname, BopData *d, char *dfname);
void read_data(const char *dfname, BopData *d);
