namespace reader
{
#define CBUFSIZE 256
    
    long n;

    int nvars;
    struct Cbuf {char c[CBUFSIZE];} *vars;
    enum Type {FLOAT, DOUBLE, ASCII} type;
    
    float *fdata;
    double *ddata;
}
