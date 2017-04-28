namespace reader
{
#define xstr(s) str(s)
#define str(s) #s

#define CBUFSIZE 256

#define ERR(...) do {                                     \
        fprintf(stderr,"(reader)" __VA_ARGS__);           \
        exit(1);                                          \
    } while(0)

    void init()
    {
        n = -1;
        nvars = 6;
        data = NULL;
    }

    void finalize()
    {
        if (data) delete[] data;
    }

    static void get_path(const char *full, char *path)
    {
        #define SEP '/'
        int i = strlen(full);
        while (--i >= 0 && full[i] != SEP);

        if (i)
        memcpy(path, full, (i+1)*sizeof(char));
    }

    template<typename real>
    static long nvals(FILE* fd)  /* return a number of real in the file */
    {
        long end, curr;
        curr = ftell(fd);
        fseek(fd, 0, SEEK_END); end = ftell(fd);
        fseek(fd, curr, SEEK_SET); /* go back */
        return (end - curr) / sizeof(real);
    }

    template<typename real>
    static long read_values(const char *fn)
    {
        FILE *f = fopen(fn, "r");

        if (f == NULL)
        ERR("could not open <%s>\n", fn);

        const long nreals = nvals<float>(f);

        data = new real[nreals];
        
        fread(data, sizeof(real), nreals, f); 
        
        fclose(f);

        return nreals / n;
    }
    
    void read(const char *fnbop)
    {
        FILE *fh = fopen(fnbop, "r");

        if (fh == NULL)
        ERR("could not open <%s>\n", fnbop);

        // parse n
        if (fscanf(fh, " %ld\n", &n) != 1)
        ERR("wrong format\n");

        printf("n = %ld\n", n);

        char cbuf[CBUFSIZE] = {0};

        // parse datafile name
        if (fscanf(fh, "DATA_FILE: %" xstr(CBUFSIZE) "s", cbuf) != 1)
        ERR("could not read data file name\n");

        char fnval[CBUFSIZE] = {0};
        get_path(fnbop, fnval);
        strcat(fnval, cbuf);
        
        // read datafile
        nvars = read_values<float>(fnval);

        printf("(reader) found %ld fields\n", nvars);
        
        fclose(fh);
    }

}
