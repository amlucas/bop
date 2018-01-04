#define BPC(ans) do {                           \
        BopStatus s = (ans);                    \
        if (!bop_success(s)) {                  \
            fprintf(stderr, ":%s:%d: %s\n%s\n", \
                    __FILE__, __LINE__,         \
                    bob_report_error_desc(s),   \
                    bob_report_error_mesg());   \
            exit(1);                            \
        }} while (0)
