#include "headers.h"

void pwd(int argc, char *argv[])
{
    if (argc == 1)
    {
        char *CurrentWorkingDirectory = (char *)malloc(1000 * sizeof(char));
        getcwd(CurrentWorkingDirectory, 1000);
        printf("%s\n", CurrentWorkingDirectory);
    }
    else
    {
        printf("Too many arguments\n");
    }
}