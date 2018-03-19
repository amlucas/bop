#define xstr(s) str(s)
#define str(s) #s

#define ERR(fmt, ...) do {                              \
        sprintf(bop_error_msg, ":%s:%d: " fmt,          \
                __FILE__, __LINE__, ##__VA_ARGS__);     \
    } while (0)
