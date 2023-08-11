    #include <string.h>

    void
    boom(char *str)
    {
    char buffer[128];
    strcpy(buffer, str);  // The horror…
    }

    int
    main(int argc, char *argv[])
    {
    boom(argv[1]);
    return 0;
    }
