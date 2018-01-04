/* use define instead of enum because of implementation details */
/* see str macro                                                */
#define CBUFSIZE 256

/* buffer for small strings */
struct Cbuf {
    char c[CBUFSIZE];
};

/* structure to describe bop file */
struct BopData {
    long n;              /* total number of particles          */
    int nvars;           /* number of variables per particle   */
    char vars[CBUFSIZE]; /* variable descriptions              */
    BopType type;    
    void *data;          /* particle data                      */
    int nrank;           /* (opt) number of ranks              */
    long *nprank;        /* (opt) number of particles per rank */
};

/* error handling */
extern char bop_error_msg[1024];
enum {
    BOP_SUCCESS,
    BOP_BADALLOC,
    BOP_BADFILE,
    BOP_WRONGVAR,
    BOP_MISMATCH,
    BOP_WFORMAT,
    BOP_NULLPTR,
    _BOP_NERR
};
