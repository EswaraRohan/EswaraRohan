# include "headers.h"

void signalHandler(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Invalid arguments\n");
        return;
    }
    int pid_index = atoi(argv[1]);
    int signal = atoi(argv[2]);
    if (pid_index <= 0 || pid_index > PIDS_COUNT)
    {
        printf("Invalid pid index\n");
        return;
    }
    if (signal < 0 || signal > 31)
    {
        printf("Invalid signal\n");
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

    int status;
    // send the signal to the process PIDS[idx]
    if (kill(PIDS[idx], signal) == -1)
    {
        perror("kill");
    }
    
}

void TSTP_handler(int signum) {
    signal(SIGTSTP, TSTP_handler);
    
    // stop the process with pid fPID
    if (fPID != -1)
    {
        if (kill(fPID, SIGTSTP) == -1)
        {
            perror("kill");
        }
        else{
            // add the process to the background
            PIDS[PIDS_COUNT] = fPID;
            strcpy(PIDS_NAME[PIDS_COUNT], fPID_NAME);

            PIDS_COUNT++;
            fPID = -1;
            
        }
    }
}
