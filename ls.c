#include "headers.h"

// case insensitive sorting
void sortList()
{
    // for (int i = 0; i < TO_PRINT_COUNT; i++)
    // {
    //     for (int j = i + 1; j < TO_PRINT_COUNT; j++)
    //     {
    //         if (strcasecmp(TO_PRINT[i], TO_PRINT[j]) > 0)
    //         {
    //             char temp[1000];
    //             strcpy(temp, TO_PRINT[i]);
    //             strcpy(TO_PRINT[i], TO_PRINT[j]);
    //             strcpy(TO_PRINT[j], temp);
    //         }
    //     }
    // }
}

void printList()
{
    // for (int i = 0; i < TO_PRINT_COUNT; i++)
    // {
    //     printName(TO_PRINT[i]);
    // }
}

// total int = Sum of (physical_blocks_in_use) * physical_block_size/ls_block_size) for each file.
long int get_total_blocks(char *path)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    struct stat statbuf;
    long int total = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        lstat(entry->d_name, &statbuf);
        total += statbuf.st_blocks;
    }
    long int ls_block_size = 1024;

    long int pysical_block_size;
    //%B value on stat
    pysical_block_size = statbuf.st_blksize;
    total = (total * pysical_block_size) / ls_block_size;
    closedir(dir);
    return total;
}

void ls(int argc, char *argv[])
{
    int a = 0, l = 0, j = 0;
    char **names = (char **)malloc(argc * sizeof(char *));
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-a") == 0)
        {
            a = 1;
        }
        else if (strcmp(argv[i], "-l") == 0)
        {
            l = 1;
        }
        else if (strcmp(argv[i], "-al") == 0 || strcmp(argv[i], "-la") == 0)
        {
            a = 1;
            l = 1;
        }
        else if (argv[i][0] == '~')
        {
            names[j] = (char *)malloc(1000 * sizeof(char));
            strcpy(names[j], HOME);
            strcat(names[j], argv[i] + 1);
            j++;
        }
        else
        {
            names[j] = (char *)malloc(1000 * sizeof(char));
            strcpy(names[j], argv[i]);
            j++;
        }
    }
    if (j == 0)
    {
        names[0] = (char *)malloc(1000 * sizeof(char));
        strcpy(names[0], ".");
        j++;
    }
    listInformation(names, j, a, l);
}

void listInformation(char **names, int num_names, int a, int l)
{
    for (int i = 0; i < num_names; i++)
    {
        char TO_PRINT[1000][1000];
        int TO_PRINT_COUNT = 0;
        if (num_names > 1)
        {
            printf("%s:\n", names[i]);
        }
        if (l == 0)
        {
            DIR *d;
            struct dirent *dir;
            // if names[i] is not a file
            if (opendir(names[i]) != NULL)
            {

                d = opendir(names[i]);
                if (d)
                {

                    while ((dir = readdir(d)) != NULL)
                    {
                        if (a == 0)
                        {

                            if (dir->d_name[0] != '.')
                            {
                                printName(dir->d_name, names[i]);
                            }
                        }
                        else
                        {
                            printName(dir->d_name, names[i]);
                        }
                    }
                    closedir(d);
                }
            }
            // check if names[i] is a file
            else
            {
                struct stat fileStat;
                if (stat(names[i], &fileStat) < 0)
                {
                    perror("stat");
                    exit(1);
                }
                printName(names[i], ".");
            }
        }
        else if (l == 1)
        {
            DIR *d;
            struct dirent *dir;
            // if names[i] is not a file
            if (opendir(names[i]) != NULL)
            {
                d = opendir(names[i]);
                if (d)
                {

                    printf("total %ld\n", get_total_blocks(names[i]));
                    while ((dir = readdir(d)) != NULL)
                    {
                        if (a == 0)
                        {
                            if (dir->d_name[0] != '.')
                            {
                                listFileInformation(dir->d_name, names[i]);
                            }
                        }
                        else
                        {
                            listFileInformation(dir->d_name, names[i]);
                        }
                    }
                }
                closedir(d);
            }
            // check if names[i] is a file
            else
            {
                listFileInformation(names[i], ".");
            }
        }
        if (num_names > 1 && i != num_names - 1)
        {
            printf("\n");
        }
    }
}

void listFileInformation(char *filename, char *current_directory)
{
    // print the file type, permissions, number of links, owner, group, size, last modified time, and name
    char *path = (char *)malloc(1000 * sizeof(char));
    strcpy(path, current_directory);
    if (path[strlen(path) - 1] != '/')
        strcat(path, "/");
    strcat(path, filename);
    struct stat fileStat;
    if (stat(path, &fileStat) < 0)
    {
        perror("stat");
        exit(1);
    }
    char *outputBuffer = (char *)malloc(1000 * sizeof(char));

    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf(" %ld", fileStat.st_nlink);
    printf(" %s", getpwuid(fileStat.st_uid)->pw_name);
    printf(" %s", getgrgid(fileStat.st_gid)->gr_name);
    printf(" %ld", fileStat.st_size);
    // time in mon dd hh:mm format if less than 6 months old, else in yyyy format
    char time[80];
    strftime(time, 80, "%b %d %H:%M", localtime(&fileStat.st_mtime));
    printf(" %s ", time);
    printName(filename, current_directory);
}

// prints the name in green if executable, white if files or blue if directories
void printName(char *name, char *current_directory)
{
    char *path = (char *)malloc(1000 * sizeof(char));
    strcpy(path, current_directory);
    if (path[strlen(path) - 1] != '/')
        strcat(path, "/");
    strcat(path, name);
    struct stat fileStat;
    if (stat(path, &fileStat) < 0)
    {
        perror("stat");
        exit(1);
    }
    if (S_ISDIR(fileStat.st_mode))
    {
        printf("\033[1;34m");
        printf("%s\n", name);
        printf("\033[0m");
    }
    else if (fileStat.st_mode & S_IXUSR)
    {
        printf("\033[1;32m");
        printf("%s\n", name);
        printf("\033[0m");
    }
    else
    {
        printf("%s\n", name);
    }
}