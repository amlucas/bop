#define BPC(ans) do {                           \
        BopStatus s = (ans);                    \
        if (s != BOP_SUCCESS) {                 \
            fprintf(stderr, ":%s:%d: %s\n%s\n", \
                    __FILE__, __LINE__,         \
                    bop_report_error_desc(s),   \
                    bop_report_error_mesg());   \
            exit(1);                            \
        }} while (0)
