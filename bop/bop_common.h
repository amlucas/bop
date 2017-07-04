#ifndef BOP_COMMON_H
#define BOP_COMMON_H

#define CBUFSIZE 256

struct Cbuf {char c[CBUFSIZE];};
enum Type {FLOAT, DOUBLE, INT, FASCII, IASCII};

struct BopData {
    long n;

    int nvars;
    Cbuf *vars;
    Type type;
    
    float  *fdata;
    double *ddata;
    int    *idata;
};

void init(BopData *d);
void finalize(BopData *d);
void summary(const BopData *d);
void concatenate(const int nd, const BopData *dd, BopData *dall);

#endif // BOP_COMMON_H
