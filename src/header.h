namespace bop_header {
enum {SERIAL = -1};
BopStatus read_header(int rank, const char *fname, /**/ char *dfname, BopData *d);
BopStatus write_header(const char *fhname, const char *fdname, const BopData *d);
}
