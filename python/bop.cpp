#include <bop_common.h>
#include <bop_serial.h>

extern  "C" {

    /* common */
    
    BopStatus cbop_ini(BopData **d) {return bop_ini(d);}
    BopStatus cbop_alloc(BopData *d) {return bop_alloc(d);}
    BopStatus cbop_fin(BopData *d) {return bop_fin(d);}

    BopStatus cbop_set_n(long n, BopData *d) {return bop_set_n(n, d);}
    BopStatus cbop_set_vars(int n, const char *vars, BopData *d) {return bop_set_vars(n, vars, d);}
    BopStatus cbop_set_type(BopType type, BopData *d) {return bop_set_type(type, d);}
    void*     cbop_get_data(BopData *d) {return bop_get_data(d);}

    BopStatus cbop_get_n(const BopData *d, long *n) {return bop_get_n(d, n);}
    BopStatus cbop_get_nvars(const BopData *d, int *n) {return bop_get_nvars(d, n);}
    BopStatus cbop_get_vars(const BopData *d, Cbuf *vars) {return bop_get_vars(d, vars);}
    BopStatus cbop_get_type(const BopData *d, BopType *type) {return bop_get_type(d, type);}
    const void* cbop_get_const_data(const BopData *d) {return bop_get_data(d);}

    int          cbop_success(BopStatus status) {return bop_success(status);}
    const char * cbop_report_error_desc(BopStatus status) {return bop_report_error_desc(status);}
    char *       cbop_report_error_mesg() {return bop_report_error_mesg();}

    /* serial */

    BopStatus cbop_write_header(const char *name, const BopData *d) {
        return bop_write_header(name, d);
    }

    BopStatus cbop_write_values(const char *name, const BopData *d) {
        return bop_write_values(name, d);
    }

    BopStatus cbop_read_header(const char *hfname, BopData *d, char *dfname) {
        return bop_read_header(hfname, d, dfname);
    }

    BopStatus cbop_read_values(const char *dfname, BopData *d) {
        return bop_read_values(dfname, d);
    }
}
