#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
    execle("/bin/ls", "/bin/ls", "-la", 0, envp);
    printf("Error on program start\n");
    exit(-1);
    
    return 0;
}