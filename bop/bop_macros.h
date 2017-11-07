#define xstr(s) str(s)
#define str(s) #s

#define ERR(...) do {                               \
        fprintf(stderr,"(reader) " __VA_ARGS__);    \
        exit(1);                                    \
    } while(0)
