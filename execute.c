#include "headers.h"

int STDIN[1000];
int STDOUT[1000];
int TOP = -1;

void execute(int argc, char **argv, int foreground)
{

    pid_t pid;
    pid = fork();

    if (pid == -1)
    {
        perror("Error in forking a new process");
    }
    else if (pid == 0)
    {
        checkRedirection(argc, argv);

        execvp(argv[0], argv);
        perror("Error");
        _exit(0);
    }
    else
    {
        if (foreground == 1)
        {

            // wait till the child process is finished
            int status;
            // calculate the time of execution in seconds
            clock_t start = clock();

            fPID = pid;
            strcpy(fPID_NAME, argv[0]);

            waitpid(pid, &status, WUNTRACED);

            fPID = -1;
            clock_t end = clock();
            TIME_TAKEN = ((double)(end - start)) / CLOCKS_PER_SEC;
        }
        else
        {
            PIDS[PIDS_COUNT] = pid;
            strcpy(PIDS_NAME[PIDS_COUNT], argv[0]);
            PIDS_COUNT++;
            printf("[%d] %d\n", PIDS_COUNT, pid);
        }
    }
}

void checkRedirection(int argc, char **argv)
{
    int input = 0, output = 0, append = 0;
    char *input_file = (char *)malloc(1000 * sizeof(char));
    char *output_file = (char *)malloc(1000 * sizeof(char));
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "<") == 0)
        {
            input = 1;
            strcpy(input_file, argv[i + 1]);
            argv[i] = NULL;
        }
        else if (strcmp(argv[i], ">") == 0)
        {
            output = 1;
            strcpy(output_file, argv[i + 1]);
            argv[i] = NULL;
        }
        else if (strcmp(argv[i], ">>") == 0)
        {
            append = 1;
            strcpy(output_file, argv[i + 1]);
            argv[i] = NULL;
        }
    }
    if (input == 1)
    {
        // display error if the input file does not exist
        struct stat fileStat;
                if (stat(input_file, &fileStat) < 0)
                {
                    perror("stat");
                    exit(1);
                }

        int fd0 = open(input_file, O_RDONLY, 0);
        dup2(fd0, 0);
        close(fd0);
    }
    if (output == 1)
    {
        int fd1 = creat(output_file, 0644);
        dup2(fd1, 1);
        close(fd1);
    }
    if (append == 1)
    {
        int fd2 = open(output_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
        dup2(fd2, 1);
        close(fd2);
    }
}

void setStandardInputOutput()
{
    STDIN[++TOP] = dup(0);
    STDOUT[TOP] = dup(1);
}

void RestoreStandardInputOutput()
{
    dup2(STDIN[TOP], 0);
    dup2(STDOUT[TOP--], 1);
}

void pipedExecute(int argc, char **argv, int foreground)
{
    int num_pipes = 0, new_argc = 0;
    char **new_argv = (char **)malloc(1000 * sizeof(char *));

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "|") == 0)
        {
            num_pipes++;
            argv[i] = NULL;
        }
    }
    ++num_pipes;
    setStandardInputOutput();

    for (int i = 0; i < num_pipes; i++)
    {
        int pipefd[2];

        if (pipe(pipefd) == -1)
        {
            perror("Error in creating pipe");
            return;
        }

        if (i == num_pipes - 1)
        {
            dup2(STDOUT[TOP], 1);
            close(STDOUT[TOP]);
        }
        else
        {
            dup2(pipefd[1], 1);
            close(pipefd[1]);
        }

        while (argv[new_argc] != NULL)
        {
            new_argc++;
        }

        execute(new_argc, argv, foreground);

        argv += new_argc + 1;
        new_argc = 0;

        if (i == num_pipes - 1)
        {
            dup2(STDIN[TOP--], 0);
            close(STDIN[TOP + 1]);
        }
        else
        {
            dup2(pipefd[0], 0);
            close(pipefd[0]);
        }
    }
    RestoreStandardInputOutput();
}