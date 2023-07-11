// discover <target_dir> <type_flags> <file_name>
#include "headers.h"

void discover(int argc, char *argv[])
{
    int f = 0, d = 0;
    char target_dir[1000], name[1000];
    target_dir[0] = '.';
    target_dir[1] = '/';
    target_dir[2] = '\0';
    name[0] = '\0';
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-f") == 0)
        {
            f = 1;
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            d = 1;
        }
        else
        {
            if (argv[i][0] == '"')
            {
                int j;
                for (j = 0; j < strlen(argv[i]) - 2; j++)
                {
                    name[j] = argv[i][j + 1];
                }
                name[j] = '\0';
            }
            else
            {
                strcpy(target_dir, argv[i]);
            }
        }
    }
    if (target_dir[0] == '~')
    {
        char temp[1000];
        strcpy(temp, HOME);
        strcat(temp, target_dir + 1);
        strcpy(target_dir, temp);
    }
    printDiscovered(target_dir, name, f, d);
}

// function imitating find command in linux
void printDiscovered(char *target_dir, char *name, int f, int d)
{
    char *path = (char *)malloc(1000 * sizeof(char));
    if (target_dir[strlen(target_dir) - 1] != '/')
    {
        strcat(target_dir, "/");
    }

    if (d == 0 && f == 0)
    {
        d = 1;
        f = 1;
    }
    DIR *p_dir = opendir(target_dir);
    struct dirent *p_dirent;
    if (p_dir == NULL)
    {
        printf("Error: Directory %s not found\n", target_dir);
        return;
    }
    else
    {
        while ((p_dirent = readdir(p_dir)) != NULL)
        {
            if (strcmp(p_dirent->d_name, "..") == 0 || strcmp(p_dirent->d_name, ".") == 0)
            {
                continue;
            }
            strcpy(path, target_dir);
            strcat(path, p_dirent->d_name);
            if (opendir(path) != NULL)
            {
                if (d == 1)
                {
                    if (name[0] != '\0')
                    {
                        if (strcmp(name, p_dirent->d_name) == 0)
                        {
                            printf("%s\n", path);
                        }
                    }
                    else
                    {
                        printf("%s\n", path);
                    }
                }
            }
            else
            {
                if (f == 1)
                {
                    if (name[0] != '\0')
                    {
                        if (strcmp(name, p_dirent->d_name) == 0)
                        {
                            printf("%s\n", path);
                        }
                    }
                    else
                    {
                        printf("%s\n", path);
                    }
                }
            }
        }
    }
}