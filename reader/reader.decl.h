#define CBUFSIZE 256

struct Cbuf {char c[CBUFSIZE];};
enum Type {FLOAT, DOUBLE, ASCII};

namespace reader
{    
    long n;

    int nvars;
    Cbuf *vars;
    Type type;
    
    float *fdata;
    double *ddata;
}
