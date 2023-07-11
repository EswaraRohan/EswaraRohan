#include "headers.h"

void printHistory(int argc, char *argv[])
{
    if (argc == 1)
    {
        // print the history from the file
        FILE *fp = fopen("history.txt", "r");
        char *line = (char *)malloc(1000 * sizeof(char));
        char *commands[21];
        int i = 0;
        while (fgets(line, 1000, fp) != NULL)
        {
            commands[i] = (char *)malloc(1000 * sizeof(char));
            line[strlen(line) - 1] = '\0';
            strcpy(commands[i], line);
            i++;
        }
        fclose(fp);

        int j = i - 10;
        if (j < 0)
            j = 0;

        while (j < i)
        {
            printf("%s\n", commands[j]);
            j++;
        }
    }
    else
    {
        printf("Too many arguments\n");
    }
}
