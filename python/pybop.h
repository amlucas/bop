struct BopData;

class PyBop {
public:
    PyBop();
    ~PyBop();

    void alloc();
    void set_n(long n);
    //void set_vars(int n, const char *vars);

    long get_n();
    int get_nvars();
    //std::vector<std::string> get_vars();
    //const void* bop_get_data();

    
private:
    BopData *d;
};
