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

enum {
    BOP_SUCCESS,
    BOP_BADALLOC,
    BOP_BADFILE,
    _BOP_NERR
};
typedef int BopStatus;

extern char bop_error_msg[1024];

BopStatus bop_alloc(BopData *d);
BopStatus bop_free(BopData *d);

void bop_extract_vars(const BopData *d, /**/ Cbuf *vars);

void bop_summary(const BopData *d);
void bop_concatenate(const int nd, const BopData *dd, BopData *dall);


const char * bob_report_error_desc(BopStatus status);
char *       bob_report_error_mesg();
