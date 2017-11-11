namespace bop_header {
BopStatus read_header(const char *fname, /**/ char *dfname, BopData *d);
BopStatus write_header(const char *fhname, const char *fdname, const BopData *d);
}
