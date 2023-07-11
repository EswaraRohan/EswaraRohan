// print the pid, process status, memory and executable path of the process with or without the given pid
#include "headers.h"

void printPinfo(int PID)
{
    char *pid = (char *)malloc(1000 * sizeof(char));
    sprintf(pid, "%d", PID);
    char *path = (char *)malloc(1000 * sizeof(char));
    sprintf(path, "/proc/%s", pid);
    char *status = (char *)malloc(1000 * sizeof(char));
    sprintf(status, "%s/status", path);
    char *stat = (char *)malloc(1000 * sizeof(char));
    sprintf(stat, "%s/stat", path);
    char *exe = (char *)malloc(1000 * sizeof(char));
    sprintf(exe, "%s/exe", path);
    char *memory = (char *)malloc(1000 * sizeof(char));
    sprintf(memory, "%s/statm", path);
    char *line = (char *)malloc(1000 * sizeof(char));
    FILE *fp = fopen(status, "r");
    printf("pid : %s\n", pid);
    while (fgets(line, 1000, fp) != NULL)
    {
        if (strstr(line, "State") != NULL)
        {
            line += 7;
            printf("process status : %c", line[0]);
            break;
        }
    }
    fclose(fp);
    // print whether the process is running in background or foreground
    fp = fopen(stat, "r");
    while (fgets(line, 1000, fp) != NULL)
    {
        char *token = strtok(line, " ");
        int i = 0;
        while (token != NULL)
        {
            if (i == 2)
            {
                if (strcmp(token, "R") == 0)
                {
                    printf("+\n");
                }
                else
                {
                    printf("\n");
                }
                break;
            }
            i++;
            token = strtok(NULL, " ");
        }
    }
    fclose(fp);
    fp = fopen(memory, "r");
    while (fgets(line, 1000, fp) != NULL)
    {
        printf("memory : %s", line);
        break;
    }
    fclose(fp);
    char *executablePath = (char *)malloc(1000 * sizeof(char));
    readlink(exe, executablePath, 1000);
    printf("executable path : %s\n", executablePath);

    free(pid);
    free(path);
    free(status);
    free(stat);
    free(exe);
    free(memory);
    free(executablePath);
}

void pinfo(int argc, char *argv[])
{
    if (argc == 1)
    {
        printPinfo(getpid());
    }
    else if (argc == 2)
    {
        printPinfo(atoi(argv[1]));
    }
    else
    {
        printf("Too many arguments\n");
    }
}