#include "headers.h"

void fg(int argc, char **argv)
{
    pipedExecute(argc, argv, 1);
}

void INT_handler(int signum)
{
    signal(SIGINT, INT_handler);
    
    // stop the process with pid fPID
    if (fPID != -1)
    {
        if (kill(fPID, SIGINT) == -1)
        {
            perror("kill");
        }
    }
    
}