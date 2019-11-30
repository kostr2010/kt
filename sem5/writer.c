#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[], char* envp[]) {
    int fd = atoi(argv[1]);

    char str[] = "AYAYAYA Konstantin\n";

    if (write(fd, str, strlen(str)) != strlen(str)) {
        printf("cant write to file\n");
        exit(-1);
    }

    printf("i am writer and i wrote:\n"
           "<%s>\n\n", str);

    close(fd);

    return 0;
}