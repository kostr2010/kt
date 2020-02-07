#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "shmem.h"

//####################//

const int MAX_LEN = 4000;

//####################//

int main() {
    int fd = open("task2-writer.c", O_RDONLY);
    if (fd == -1) {
        printf("can't open program's code!\n");
        exit(-1);
    }

    int len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    if (len >= MAX_LEN - 1) {
        printf("too long!\n");
        exit(-1);
    }
    
    char* msg = calloc(len, sizeof(char));

    if (read(fd, msg, len) != len) {
        printf("can't read source code!\n");
        exit(-1);
    }

    ShMem* shmem = ShMemAlloc();

    if (ShMemInit(shmem, MAX_LEN, "task2-writer.c") != OK)
        ShMemCheck(shmem);

    memcpy(shmem->buf, msg, strlen(msg));

    if (ShMemFree(shmem) != OK)
        ShMemCheck(shmem);
}


