namespace reader
{
    enum Type {FLOAT, DOUBLE, ASCII};
    
#define xstr(s) str(s)
#define str(s) #s

#define CBUFSIZE 256

#define ERR(...) do {                                      \
        fprintf(stderr,"(reader) " __VA_ARGS__);           \
        exit(1);                                           \
    } while(0)

    void init()
    {
        n = -1;
        nvars = 6;
        fdata = NULL;
        ddata = NULL;
    }

    void finalize()
    {
        if (fdata) delete[] fdata;
        if (ddata) delete[] ddata;
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
    static long read_values(const char *fn, real *data)
    {
        FILE *f = fopen(fn, "r");

        if (f == NULL)
        ERR("could not open <%s>\n", fn);

        const long nreals = nvals<real>(f);

        data = new real[nreals];
        
        fread(data, sizeof(real), nreals, f); 
        
        fclose(f);

        return nreals / n;
    }

    static void readline(FILE *f, char *buf) // read full line unless it excess CBUFSIZE chars 
    {
        memset(buf, 0, CBUFSIZE * sizeof(char));

        if (fscanf(f, " %[^\n]" xstr(CBUFSIZE) "c", buf) != 1)
        ERR("line too long\n");
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

        char cbuf[CBUFSIZE] = {0}, line[CBUFSIZE] = {0};

        // parse datafile name
        readline(fh, line);
        
        if (sscanf(line, "DATA_FILE: %" xstr(CBUFSIZE) "s", cbuf) != 1)
        ERR("could not read data file name\n");

        char fnval[CBUFSIZE] = {0};
        get_path(fnbop, fnval);
        strcat(fnval, cbuf);

        // parse data format
        readline(fh, line);
        
        Type type;
        {        
            if (sscanf(line, "DATA_FORMAT: %" xstr(CBUFSIZE) "s", cbuf) != 1)
            ERR("could not read data file format\n");

            if      (strcmp(cbuf,  "float") == 0) type = FLOAT;
            else if (strcmp(cbuf, "double") == 0) type = DOUBLE;
            else if (strcmp(cbuf,  "ascii") == 0) type = ASCII;
            else     ERR("wrong DATA_FORMAT\n");
        }
        
        // read datafile
        switch (type)
        {
        case FLOAT:  nvars = read_values<float> (fnval, fdata); break;
        case DOUBLE: nvars = read_values<double>(fnval, ddata); break;
        case ASCII: ERR("Not implemented\n");
        };

        printf("(reader) found %d fields\n", nvars);

        // read variables (TODO)
        
        fclose(fh);
    }

}
