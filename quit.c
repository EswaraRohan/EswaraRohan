#include "headers.h"

void quit(int argc, char *argv[])
{
    if (argc == 1)
    {
        exit(0);
    }
    else
    {
        printf("Too many arguments\n");
    }
}