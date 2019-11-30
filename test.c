////////////////////////////////////////////
// Nazarov K 812 group task 2             //
//                                        //
// program looks for print argument.      //
// if found, next argument will be passed // 
// as the argument of auxillary program   //
// called aux.c                           //
////////////////////////////////////////////

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PrintEnviroment(char* argv[], char* envp[]);   // Prints args of command line and enivromental params.

int main(int argc, char* argv[], char* envp[]) {
    int a = 0;

    PrintEnviroment(argv, envp);

    if (strcmp("print", argv[1]) == 0)
        execle("./aux", "./aux", argv[2],  NULL, envp);

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
