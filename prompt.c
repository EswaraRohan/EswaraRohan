#include "headers.h"

double TIME_TAKEN = 0;

char *getUserName(void)
{
    uid_t uid = getuid();
    return getpwuid(uid)->pw_name;
}

char *getHostName(void)
{
    struct utsname *buffer = (struct utsname *)malloc(sizeof(struct utsname));
    uname(buffer);
    return buffer->nodename;
}

void prompt()
{
    char *CurrentWorkingDirectory = (char *)malloc(1000 * sizeof(char));
    getcwd(CurrentWorkingDirectory, 1000);

    if (strcmp(CurrentWorkingDirectory, HOME) == 0)
    {
        strcpy(CurrentWorkingDirectory, "~");
    }

    printf("<%s@%s:%s", getUserName(), getHostName(), CurrentWorkingDirectory);
    TIME_TAKEN >= 1 ? printf("took %lds>", (long int)TIME_TAKEN) : printf(">");
}
