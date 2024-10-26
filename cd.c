#include "headers.h"


char HOME[1000];
char PreviousWorkingDirectory[1000];

void cd(int argc, char *argv[])
{
    char *CurrentWorkingDirectory = (char *)malloc(1000 * sizeof(char));
    getcwd(CurrentWorkingDirectory, 1000);

    if (argc == 1)
    {
        chdir(HOME);
        strcpy(PreviousWorkingDirectory, CurrentWorkingDirectory);
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-") == 0)
        {
            chdir(PreviousWorkingDirectory);
        }
        else if (argv[1][0] == '~')
        {
            char *temp = (char *)malloc(1000 * sizeof(char));
            strcpy(temp, HOME);
            strcat(temp, argv[1] + 1);
            printf("%s\n", temp);
            chdir(temp);
        }
        else
        {
            char *temp = (char *)malloc(100000 * sizeof(char));
            strcpy(temp, argv[1]);

            if (chdir(temp) == -1)
            {
                printf("No such file or directory\n");
            }

            free(temp);
        }

        strcpy(PreviousWorkingDirectory, CurrentWorkingDirectory);
    }
    else
    {
        printf("Too many arguments\n");
    }
}