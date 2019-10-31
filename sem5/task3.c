#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd = -1;

    if((fd = open("task3.txt", O_RDONLY)) < 0) {
        printf("cant open file\n");
        exit(-1);
    }

    int len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char* buf = calloc(len, sizeof(char));

    if (read(fd, buf, len) != len) {
        printf("cant read file\n");
        exit(-1);
    }

    printf("%s\n", buf);

    free(buf);
    close(fd);

    return 0;
}   
