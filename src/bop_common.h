/* use define instead of enum because of implementation details */
#define CBUFSIZE 256

struct Cbuf {char c[CBUFSIZE];};

struct BopData {
    long n;              /* total number of particles        */
    int nvars;           /* number of variables per particle */
    char vars[CBUFSIZE]; /* variable descriptions            */
    enum Type {FLOAT, DOUBLE, INT, FASCII, IASCII} type;    
    void *data;          /* particle data                    */
    int nrank;           /* (opt) number of ranks            */
    long *nprank;        /* number of particle on each rank  */
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
typedef int BopStatus;

extern char bop_error_msg[1024];

BopStatus bop_alloc(BopData *d);
BopStatus bop_free(BopData *d);

BopStatus bop_extract_vars(const BopData *d, /**/ Cbuf *vars);

BopStatus bop_summary(const BopData *d);
BopStatus bop_concatenate(const int nd, const BopData *dd, BopData *dall);


const char * bob_report_error_desc(BopStatus status);
char *       bob_report_error_mesg();
