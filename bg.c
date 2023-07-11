#include "headers.h"

void bg(int argc, char **argv)
{
    pipedExecute(argc, argv, 0);
}

// bg 3: restart the process with pid index 3 in the background if it is not running
void restartBackgroundProcess(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Invalid arguments\n");
        return;
    }
    int pid_index = atoi(argv[1]);
    if (pid_index <= 0 || pid_index > PIDS_COUNT)
    {
        printf("Invalid pid index\n");
        return;
    }
    assignPIDS_INDEX();
    sortPIDS();

    int idx;
    for (int i = 0; i < PIDS_COUNT; i++)
    {
        if (PIDS_INDEX[i] == pid_index)
        {
            idx = i;
            break;
        }
    }
    
    // change the state of the process PIDS[idx] to running
    // read the status of the process from /proc/[pid]/stat
    char path[1000];
    sprintf(path, "/proc/%d/stat", PIDS[idx]);
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
        // send the signal SIGCONT to the process PIDS[idx]
        if (kill(PIDS[idx], SIGCONT) == -1)
        {
            perror("kill");
        }
    }

   }

// fg 3: bring the process with pid index 3 to the foreground and change its state to running
void bringToForeground(int argc, char* argv[])
{

    restartBackgroundProcess(argc, argv);
if (argc != 2)
    {
        printf("Invalid arguments\n");
        return;
    }
    int pid_index = atoi(argv[1]);
    if (pid_index <= 0 || pid_index > PIDS_COUNT)
    {
        printf("Invalid pid index\n");
        return;
    }
    assignPIDS_INDEX();
    sortPIDS();

    int idx;
    for (int i = 0; i < PIDS_COUNT; i++)
    {
        if (PIDS_INDEX[i] == pid_index)
        {
            idx = i;
            break;
        }
    }
    
    // change the state of the process PIDS[idx] to running
    // read the status of the process from /proc/[pid]/stat
    char path[1000];
    sprintf(path, "/proc/%d/stat", PIDS[idx]);
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        perror("fopen");
        return;
    }
    char status;
    fscanf(fp, "%*d %*s %c", &status);
    fclose(fp);
    char status1 = status;
    if (status1 == 'T')
    {
        // send the signal SIGCONT to the process PIDS[idx]
        if (kill(PIDS[idx], SIGCONT) == -1)
        {
            perror("kill");
        }
    }

    fPID = PIDS[idx];

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU,SIG_IGN);

    // bring the process to the foreground
    if (tcsetpgrp(STDIN_FILENO, PIDS[idx]) == -1)
    {
        perror("tcsetpgrp");
    }

    // wait for the process to finish
    int status2;
    if (waitpid(PIDS[idx], &status2, WUNTRACED) == -1)
    {
        perror("waitpid");
    }
    fPID = -1;

    // get the terminal back
    if (tcsetpgrp(STDIN_FILENO, getpgid(0)) == -1)
    {
        perror("tcsetpgrp");
    }

    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    for (int j = idx; j < PIDS_COUNT - 1; j++)
        {
            PIDS[j] = PIDS[j + 1];
            PIDS_INDEX[j] = PIDS_INDEX[j + 1];
            strcpy(PIDS_NAME[j], PIDS_NAME[j + 1]);
        }
        PIDS_COUNT--;

}