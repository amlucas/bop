#include <stdio.h>
#include <string.h>
#include "bop_common.h"
#include "bop_serial.h"

#include "pybop.h"

using namespace std;

// TODO
#define BPC(ans) ans

PyBop::PyBop()
    : d(nullptr)
{
    BPC(bop_ini(&d));
}

PyBop::~PyBop() {
    BPC(bop_fin(d));
}

void PyBop::alloc() {
    BPC(bop_alloc(d));
}

void PyBop::set_n(long n) {
    BPC(bop_set_n(n, d));
}

void PyBop::set_vars(int n, const string &vars) {
    BPC(bop_set_vars(n, vars.c_str(), d));
}

static const string type_str[] = {
    "float",
    "asciifloat",
    "double",
    "int",
    "asciiint"
};

void PyBop::set_type(const std::string &type) {
    BopType t;
    int found, i, n;
    n = sizeof(type_str) / sizeof(type_str[0]);
    
    for (found = i = 0; i < n; ++i) {
        if (type == type_str[i]) {
            t = i;
            found = 1;
        }
    }
            
    if (!found) {
        fprintf(stderr, "Wrong type given: %s\n", type.c_str());
        exit(1);
    }
    BPC(bop_set_type(t, d));
}

template <typename T>
static void copy(const vector<T> &src, BopData *d) {
    T *dst = (T*) bop_get_data(d);
    memcpy(dst, src.data(), src.size() * sizeof(T));
}

void PyBop::set_datai(const std::vector<int> &data) {
    copy(data, d);
}

void PyBop::set_dataf(const std::vector<float> &data) {
    copy(data, d);
}

void PyBop::set_datad(const std::vector<double> &data) {
    copy(data, d);
}

long PyBop::get_n() {
    long n;
    BPC(bop_get_n(d, &n));
    return n;
}

string PyBop::get_vars() {
    const char *cvars = nullptr;
    BPC(bop_get_vars(d, &cvars));
    
    return string(cvars);
}

string PyBop::get_type() {
    BopType t;
    BPC(bop_get_type(d, &t));
    return type_str[t];
}



void PyBop::write(string basename) {
    BPC(bop_write_header(basename.c_str(), d));
    BPC(bop_write_values(basename.c_str(), d));
}
