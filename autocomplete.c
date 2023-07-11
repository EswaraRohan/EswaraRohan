# include "headers.h"

struct termios orig_termios;


void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

void die(const char *s)
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    perror(s);
    exit(1);
}

int readKey()
{
    setbuf(stdout, NULL);

    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1)
    {
        if (nread == -1 && errno != EAGAIN)
            die("read");
    }
    
    return c;
    
}

void autoComplete(char *inputs, int *size)
{
    // get the last word
    char *lastWord = (char *)malloc(1000 * sizeof(char));
    int i = *size - 1;
    while (i >= 0 && inputs[i] != ' ')
    {
        i--;
    }
    i++;
    int j = 0;
    while (i < *size)
    {
        lastWord[j] = inputs[i];
        i++;
        j++;
    }
    lastWord[j] = '\0';

    // get the directory name
    // if the last word contains a / then the directory name is the substring before the last /
    // else the directory name is the current directory
    char *directoryName = (char *)malloc(1000 * sizeof(char));
    char *fileName = (char *)malloc(1000 * sizeof(char));
    int k = 0;
    int slashIndex = -1;
    while (k < j)
    {
        if (lastWord[k] == '/')
        {
            slashIndex = k;
        }
        k++;
    }
    if (slashIndex != -1)
    {
        int m = 0;
        while (m < slashIndex)
        {
            directoryName[m] = lastWord[m];
            m++;
        }
        directoryName[m] = '\0';
    }
    else
    {
        directoryName = ".";
    }

    // get the file name
    k = 0;
    ++ slashIndex;
    while (slashIndex < j)
    {
        fileName[k] = lastWord[slashIndex];
        ++slashIndex;
        ++k;
    }
    fileName[k] = '\0';
    if (k == 0)
    {
        return;
    }

    // execute the ls command
    char *args[6];
    args[0] = (char*)malloc(3);
    args[1] = (char*)malloc(1000);
    args[2] = (char*)malloc(3);
    args[3] = (char*)malloc(3);
    args[4] = (char*)malloc(10);
    args[5] = NULL;
    strcpy(args[0], "ls");
    strcpy(args[1], directoryName);
    strcpy(args[2], "-a");
    strcpy(args[3], ">");
    strcpy(args[4], "ls.txt");

    execute(5, args, 1);

    // read the contents of the file
    FILE *fp = fopen("ls.txt", "r");
    char *line = (char *)malloc(1000 * sizeof(char));
    // read line by line
    while (fgets(line, 1000, fp) != NULL)
    {
        // remove the newline character
        line[strlen(line) - 1] = '\0';
        // check if the line starts with the last word
        int l = 0;
        int flag = 1;
        k = strlen(fileName);
        while (l < k)
        {
            if (line[l] != fileName[l])
            {
                flag = 0;
                break;
            }
            l++;
        }
        if (flag == 1)
        {
           // append the rest of the line to the inputs
                while (line[l] != '\0')
                {
                    printf("%c", line[l]);
                    inputs[*size] = line[l];
                    *size = *size + 1;
                    l++;
                }
                if (opendir(line))
                {
                    printf("/");
                    inputs[*size] = '/';
                    *size = *size + 1;
                }
                else
                {
                    printf(" ");
                    inputs[*size] = ' ';
                    *size = *size + 1;
                }
                inputs[*size] = '\0';
                break;
        }
    }
    fclose(fp);
}