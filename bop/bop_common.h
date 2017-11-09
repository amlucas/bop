#define CBUFSIZE 256

struct Cbuf {char c[CBUFSIZE];};
enum Type {FLOAT, DOUBLE, INT, FASCII, IASCII};

struct BopData {
    long n;
    int nvars;
    Cbuf *vars;
    Type type;    
    void *data;
};

void bop_alloc(BopData *d);
void bop_free(BopData *d);

void summary(const BopData *d);
void concatenate(const int nd, const BopData *dd, BopData *dall);
