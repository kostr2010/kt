#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char* argv[], char* envp[]) {
    int fd = atoi(argv[1]);

    char* buf = calloc(8, sizeof(char));

    if ((read(fd, buf, 8)) != 8) {
        printf("cant read file\n");
        exit(-1);
    }

    printf("i am reader and i read:\n"
           "<%s>\n\n", buf);

    free(buf);
    close(fd);

    return 0;
}