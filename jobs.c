# include "headers.h"

pid_t PIDS[1000];
int PIDS_COUNT = 0;
char PIDS_NAME[1000][1000];
int PIDS_INDEX[1000];

pid_t fPID = -1;
char fPID_NAME[1000];


void checkBackgroundProcess()
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0)
    {
        for (int i = 0; i < PIDS_COUNT; i++)
        {
            if (PIDS[i] == pid)
            {
                if (WIFEXITED(status))
                {
                    printf("\n%s with pid %d exited normally\n", PIDS_NAME[i], pid);
                    prompt();
                }
                else
                {
                    printf("\n%s with pid %d exited abnormally\n", PIDS_NAME[i], pid);
                    prompt();
                }
                for (int j = i; j < PIDS_COUNT - 1; j++)
                {
                    PIDS[j] = PIDS[j + 1];
                    PIDS_INDEX[j] = PIDS_INDEX[j + 1];
                    strcpy(PIDS_NAME[j], PIDS_NAME[j + 1]);
                }
                PIDS_COUNT--;

            }
        }
    }
}

void assignPIDS_INDEX()
{
    for (int i = 0; i < PIDS_COUNT; i++)
    {
        PIDS_INDEX[i] = i + 1;
    }
}

// sort the PIDS_INDEX[], PIDS[] and PIDS_NAME[] arrays on the basis of PIDS_NAME[] array, alphabetically
void sortPIDS()
{
    for (int i = 0; i < PIDS_COUNT; i++)
    {
        for (int j = i + 1; j < PIDS_COUNT; j++)
        {
            if (strcmp(PIDS_NAME[i], PIDS_NAME[j]) > 0)
            {
                int temp = PIDS_INDEX[i];
                PIDS_INDEX[i] = PIDS_INDEX[j];
                PIDS_INDEX[j] = temp;

                pid_t temp_pid = PIDS[i];
                PIDS[i] = PIDS[j];
                PIDS[j] = temp_pid;

                char temp_name[1000];
                strcpy(temp_name, PIDS_NAME[i]);
                strcpy(PIDS_NAME[i], PIDS_NAME[j]);
                strcpy(PIDS_NAME[j], temp_name);
            }
        }
    }
}



// function of print all the jobs in alphabetical order
// flag r for running jobs and s for stopped jobs
// example: [1] Running emacs assign1.txt [221]
// [4] Stopped gedit [3213]


void jobs(int argc, char *argv[])
{
    assignPIDS_INDEX();
    sortPIDS();

    if (argc == 1)
    {
        // print: [1] Running/Stopped emacs assign1.txt [221]
        // read the status of the process from /proc/[pid]/stat
        for (int i = 0; i < PIDS_COUNT; i++)
        {
            char path[1000];
            sprintf(path, "/proc/%d/stat", PIDS[i]);
            FILE *fp = fopen(path, "r");
            if (fp == NULL)
            {
                perror("fopen");
                return;
            }
            char status;
            fscanf(fp, "%*d %*s %c", &status);
            fclose(fp);
            if (status == 'T')
            {
                printf("[%d] Stopped %s [%d]\n", PIDS_INDEX[i], PIDS_NAME[i], PIDS[i]);
            }
            else
            {
                printf("[%d] Running %s [%d]\n", PIDS_INDEX[i], PIDS_NAME[i], PIDS[i]);
            }
        }
        
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "-r") == 0)
        {
            // print: [1] Running emacs assign1.txt [221]
            // read the status of the process from /proc/[pid]/stat
            for (int i = 0; i < PIDS_COUNT; i++)
            {
                char path[1000];
                sprintf(path, "/proc/%d/stat", PIDS[i]);
                FILE *fp = fopen(path, "r");
                if (fp == NULL)
                {
                    perror("fopen");
                    return;
                }
                char status;
                fscanf(fp, "%*d %*s %c", &status);
                fclose(fp);
                if (status == 'T')
                {
                    continue;
                }
                else
                {
                    printf("[%d] Running %s [%d]\n", PIDS_INDEX[i], PIDS_NAME[i], PIDS[i]);
                }
            }
        }
        else if (strcmp(argv[1], "-s") == 0)
        {
            // print: [1] Stopped emacs assign1.txt [221]
            // read the status of the process from /proc/[pid]/stat
            for (int i = 0; i < PIDS_COUNT; i++)
            {
                char path[1000];
                sprintf(path, "/proc/%d/stat", PIDS[i]);
                FILE *fp = fopen(path, "r");
                if (fp == NULL)
                {
                    perror("fopen");
                    return;
                }
                char status;
                fscanf(fp, "%*d %*s %c", &status);
                fclose(fp);
                if (status == 'T')
                {
                    printf("[%d] Stopped %s [%d]\n", PIDS_INDEX[i], PIDS_NAME[i], PIDS[i]);
                }
                else
                {
                    continue;
                }
            }
        }
        else
        {
            printf("Invalid argument\n");
        }
    }
    else
    {
        printf("Invalid number of arguments\n");
    }
    

}
