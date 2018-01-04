/* use define instead of enum because of implementation details */
/* see str macro                                                */
#define CBUFSIZE 256

struct Cbuf {
    char c[CBUFSIZE];
};

struct BopData {
    long n;              /* total number of particles          */
    int nvars;           /* number of variables per particle   */
    char vars[CBUFSIZE]; /* variable descriptions              */
    enum Type {FLOAT, DOUBLE, INT, FASCII, IASCII} type;    
    void *data;          /* particle data                      */
    int nrank;           /* (opt) number of ranks              */
    long *nprank;        /* (opt) number of particles per rank */
};

enum {
    BOP_SUCCESS,
    BOP_BADALLOC,
    BOP_BADFILE,
    BOP_WRONGVAR,
    BOP_MISMATCH,
    BOP_WFORMAT,
    _BOP_NERR
};
