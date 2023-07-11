#include "headers.h"

// function to save the last 20 commands in a file
void saveHistory(char *command)
{
    // read all the commands from the file
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

    // append the new command to the commands if it is not same as the last command
    command[strlen(command) - 1] = '\0';
    if (strcmp(line, command) != 0 && strcmp(command, "\0") != 0)
    {
        commands[i] = (char *)malloc(1000 * sizeof(char));
        strcpy(commands[i], command);
        i++;
    }
    int j = i == 21 ? 1 : 0;

    // write the last 20 commands to the file
    fp = fopen("history.txt", "w");

    while (j < i)
    {
        fprintf(fp, "%s\n", commands[j]);
        j++;
    }

    free(line);
    fclose(fp);
}

int main()
{
    signal(SIGINT, INT_handler);
    signal(SIGTSTP, TSTP_handler);

    strcpy(PreviousWorkingDirectory, ".");
    getcwd(HOME, 1000);
    while (1)
    {
        
        int argc = 0;
        char *argv[1000];
        // argv[0] stores the command given by the user.

        FILE *fp = fopen("history.txt", "a");
        fclose(fp);

        prompt();
        checkBackgroundProcess();
        char *inputs = (char *)malloc(1000000 * sizeof(char));
        int idx = 0; char c;
        enableRawMode();
        do {
            c = readKey();

            if (c == 127) 
            { 
                if (idx > 0) {
                    inputs[--idx] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == 9)
            {
               autoComplete(inputs, &idx);
            }
            else if (c == 4)
            {
                printf("\n");
                exit(0);
            }           
            else
            {
                printf("%c", c);

                inputs[idx++] = c;
            }

        } while (inputs[idx - 1] != '\n');
        disableRawMode();
        saveHistory(inputs);
        // seperate the inputs into fg_command[] and bg_command[] on the basis of ; and &
        char *fg_command[1000];
        char *bg_command[1000];
        char part[1000];
        int fg_idx = 0, bg_idx = 0, j = 0;

        for (int i = 0; i < strlen(inputs); ++i, ++j)
        {
            if (inputs[i] != ';' && inputs[i] != '&')
            {
                part[j] = inputs[i];
            }
            if (inputs[i] == ';')
            {
                fg_command[fg_idx] = (char *)malloc(1000 * sizeof(char));
                part[j] = '\0';
                strcpy(fg_command[fg_idx], part);
                fg_idx++;
                j = -1;
            }
            else if (inputs[i] == '&')
            {
                bg_command[bg_idx] = (char *)malloc(1000 * sizeof(char));
                part[j] = '\0';
                strcpy(bg_command[bg_idx], part);
                bg_idx++;
                j = -1;
            }
        }
        fg_command[fg_idx] = (char *)malloc(1000 * sizeof(char));
        part[j] = '\0';
        strcpy(fg_command[fg_idx], part);
        fg_idx++;

        for (int i = 0; i < bg_idx; ++i)
        {
            char *token = strtok(bg_command[i], " \n");
            argc = 0;
            while (token != NULL)
            {
                argv[argc] = token;
                argc++;
                token = strtok(NULL, " \n");
            }
            argv[argc] = NULL;

            bg(argc, argv);
        }

        for (int i = 0; i < fg_idx; ++i)
        {

            char *token = strtok(fg_command[i], " \n");
            argc = 0;
            while (token != NULL)
            {
                argv[argc] = token;
                argc++;
                token = strtok(NULL, " \n");
            }
            argv[argc] = NULL;
            if (argc == 0)
            {
                continue;
            }
            else if (strcmp(argv[0], "cd") == 0)
            {
                cd(argc, argv);
            }
            else if (strcmp(argv[0], "pwd") == 0)
            {
                pwd(argc, argv);
            }
            else if (strcmp(argv[0], "pinfo") == 0)
            {
                pinfo(argc, argv);
            }
            else if (strcmp(argv[0], "q") == 0)
            {
                quit(argc, argv);
            }
            else if (strcmp(argv[0], "history") == 0)
            {
                printHistory(argc, argv);
            }
            else if (strcmp(argv[0], "discover") == 0)
            {
                discover(argc, argv);
            }
            else if (strcmp(argv[0], "jobs") == 0)
            {
                jobs(argc, argv);
            }
            else if (strcmp(argv[0], "sig") == 0)
            {
                signalHandler(argc, argv);
            }
            else if (strcmp(argv[0], "bg") == 0)
            {
                restartBackgroundProcess(argc, argv);
            }
            else if (strcmp(argv[0], "fg") == 0)
            {
                bringToForeground(argc, argv);
            }
            else
            {
                fg(argc, argv);
            }
        }
    }

    return 0;
}
