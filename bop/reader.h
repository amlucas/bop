#ifndef BOP_READER_H
#define BOP_READER_H

#define CBUFSIZE 256

struct Cbuf {char c[CBUFSIZE];};
enum Type {FLOAT, DOUBLE, ASCII};

struct ReadData {
    long n;

    int nvars;
    Cbuf *vars;
    Type type;
    
    float *fdata;
    double *ddata;
};

void init(ReadData *d);
void finalize(ReadData *d);
void read(const char *fnbop, ReadData *d);
void summary(const ReadData *d);
void concatenate(const int nd, const ReadData *dd, ReadData *dall);

#endif // BOP_READER_H
