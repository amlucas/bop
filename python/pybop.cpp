#include "bop_common.h"
#include "bop_serial.h"

#include "pybop.h"

#define BPC(ans) ans

PyBop::PyBop()
    : d(nullptr)
{
    BPC(bop_ini(&d));
}

PyBop::~PyBop() {
    BPC(bop_fin(d));
}

void PyBop::set_n(long n) {
    BPC(bop_set_n(n, d));
}

long PyBop::get_n() {
    long n;
    BPC(bop_get_n(d, &n));
    return n;
}
