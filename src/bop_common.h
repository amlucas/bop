struct Cbuf;
struct BopData;
typedef int BopStatus;

BopStatus bop_alloc(BopData *d);
BopStatus bop_free(BopData *d);

BopStatus bop_extract_vars(const BopData *d, /**/ Cbuf *vars);

BopStatus bop_summary(const BopData *d);
BopStatus bop_concatenate(const int nd, const BopData *dd, BopData *dall);

const char * bob_report_error_desc(BopStatus status);
char *       bob_report_error_mesg();
