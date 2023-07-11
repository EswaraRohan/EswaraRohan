#include <stdio.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <time.h>
#include <grp.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>
#include <ctype.h>


struct Entry
{
    char entryName[1000];
    char directory[1000];
    char information[1000];
};

extern char HOME[1000];
extern char PreviousWorkingDirectory[1000];
extern pid_t PIDS[1000];
extern int PIDS_COUNT;
extern char PIDS_NAME[1000][1000];
extern double TIME_TAKEN;
extern int PIDS_INDEX[1000];

extern pid_t fPID;
extern char fPID_NAME[1000];


void prompt();
char *getUserName();
char *getHostName();
long int get_total_blocks(char *path);
void listInformation(char **names, int num_names, int a, int l);
void listFileInformation(char *filename, char *current_directory);
void printDiscovered(char *target_dir, char *name, int f, int d);
void printName(char *name, char *current_directory);
void checkBackgroundProcess();
extern void assignPIDS_INDEX();
extern void sortPIDS();
void jobs(int argc, char *argv[]);
void signalHandler(int argc, char* argv[]);
void restartBackgroundProcess(int argc, char* argv[]);
void bringToForeground(int argc, char* argv[]);
void INT_handler(int signum);
void TSTP_handler(int signum); 



void bg(int argc, char **argv);
void fg(int argc, char **argv);
void discover(int argc, char **argv);
void echo(int argc, char **argv);
void pwd(int argc, char **argv);
void cd(int argc, char **argv);
void ls(int argc, char **argv);
void pinfo(int argc, char **argv);
void history(int argc, char **argv);
void quit(int argc, char **argv);

void printHistory(int argc, char **argv);
void execute(int argc, char **argv, int foreground);
void checkRedirection(int argc, char **argv);
void setStandardInputOutput();
void RestoreStandardInputOutput();
void pipedExecute(int argc, char **argv, int foreground);
void disableRawMode();
void enableRawMode();
void die(const char *s);
int readKey();
void autoComplete(char *inputs, int *size);




