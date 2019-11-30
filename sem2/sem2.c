#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    printf("Process id: %d\n"
           "Parent id: %d\n", pid, ppid);
}