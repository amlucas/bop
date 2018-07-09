#pragma once

#include <vector>
#include <string>

struct BopData;

class PyBop {
public:
    PyBop();
    ~PyBop();

    void alloc();
    void set_n(long n);
    void set_vars(int n, const std::string &vars);
    void set_type(const std::string &type);

    long get_n();
    std::string get_vars();
    std::string get_type();

    void set_datai(const std::vector<int>    &data);
    void set_dataf(const std::vector<float>  &data);
    void set_datad(const std::vector<double> &data);

    void write(std::string basename);
    
private:
    BopData *d;
};
