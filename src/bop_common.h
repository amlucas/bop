struct Cbuf {
    enum {SIZ=256};
    char c[SIZ];
};
struct BopData;
typedef int BopStatus;

enum BopType {BopFLOAT, BopDOUBLE, BopINT, BopFASCII, BopIASCII};

/* memory management */

BopStatus bop_ini(BopData **d);
BopStatus bop_alloc(BopData *d);
BopStatus bop_fin(BopData *d);

/* getters, setters */

BopStatus bop_set_n(long n, BopData *d);
BopStatus bop_set_vars(int n, const char *vars, BopData *d);
BopStatus bop_set_type(BopType type, BopData *d);

BopStatus bop_get_n(const BopData *d, long *n);
BopStatus bop_get_nvars(const BopData *d, int *n);
BopStatus bop_get_vars(const BopData *d, Cbuf *vars);
BopStatus bop_get_type(const BopData *d, BopType *type);

void* bop_get_data(BopData *d);

BopStatus bop_extract_vars(const BopData *d, /**/ Cbuf *vars);

/* tools */

BopStatus bop_summary(const BopData *d);
BopStatus bop_concatenate(const int nd, const BopData **dd, BopData *dall);

/* error handling */
bool         bop_success(BopStatus status);
const char * bob_report_error_desc(BopStatus status);
char *       bob_report_error_mesg();
