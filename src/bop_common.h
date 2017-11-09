#define CBUFSIZE 256

enum Type {FLOAT, DOUBLE, INT, FASCII, IASCII};
struct Cbuf {char c[CBUFSIZE];};

struct BopData {
    long n;
    int nvars;
    char vars[CBUFSIZE];
    Type type;    
    void *data;
};

void bop_alloc(BopData *d);
void bop_free(BopData *d);

void bop_extract_vars(const BopData *d, /**/ Cbuf *vars);

void bop_summary(const BopData *d);
void bop_concatenate(const int nd, const BopData *dd, BopData *dall);
