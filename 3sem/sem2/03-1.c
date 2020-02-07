#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void PrintEnviroment(char* argv[], char* envp[]);   // Prints args of command line and enivromental params.
void IdentProcess(int ret, int a);                  // Identifies whether procwss is parent or child and prints it's pid and ppid.

int main(int argc, char* argv[], char* envp[]) {
    int a = 0;

    PrintEnviroment(argv, envp);

    int ret = fork();

    IdentProcess(ret, a);

    if (ret == 0) {
        execle("./sem2", "./sem2", NULL, envp);
    }

    return 0;
}

void PrintEnviroment(char* argv[], char* envp[]) {
    printf("Command line arguments:\n");
    int i = 0;
    while (argv[i]) {
        printf("  %s\n", argv[i]);
        ++i;
    }

    printf("Enviroment arguments:\n");
    i = 0;
    while (envp[i]) {
        printf("  %s\n", envp[i]);
        ++i;
    }
}

void IdentProcess(int ret, int a) {
    int pid = getpid();
    int ppid = getppid();
    
    switch (ret) {
    case 0:
        a = a + 1;
        
        printf("Child process:\n"
               "  Pid = %d\n"
               "  Ppid = %d\n"
               "  result = %d\n",pid, ppid, a);
        break;
    case -1:
        printf("Couldn't create child process\n");
        break;
    default:
        a = a + 1;

        printf("Process:\n"
               "  Pid = %d\n"
               "  Ppid = %d\n"
               "  result = %d\n",pid, ppid, a);
        break;
    }
}