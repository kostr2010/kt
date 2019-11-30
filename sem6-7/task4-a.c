#include <stdio.h>
#include "shmem.h"
#include <unistd.h>

int main() {
    const char* pathname = "task4-a.c";

    ShMem* shmem = ShMemAlloc();

    if (ShMemInit(shmem, 6 * sizeof(int), pathname) != OK) // 3 for counters, 4 & 5 for flag, 6 for turn
        ShMemCheck(shmem);

    if(shmem->new) {
        ((int*)(shmem->buf))[0] = 1;    // counter a
        ((int*)(shmem->buf))[1] = 0;    // counter b
        ((int*)(shmem->buf))[2] = 1;    // counter all
        ((int*)(shmem->buf))[3] = 0;    // flag[0]
        ((int*)(shmem->buf))[4] = 0;    // flag[1]
        ((int*)(shmem->buf))[5] = 0;    // turn 
    } else {    // critical area
        ((int*)(shmem->buf))[3] = 1;    // process a wants to use shmem
        ((int*)(shmem->buf))[5] = 1;    // set turn to process b
        while (((int*)(shmem->buf))[4] == 1 && ((int*)(shmem->buf))[5] == 1)
            continue;

        ((int*)(shmem->buf))[0] += 1;
        sleep(5);
        ((int*)(shmem->buf))[2] += 1;

        ((int*)(shmem->buf))[3] = 0;    // process a doesn't want to use shmem
    }

    printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
           ((int*)(shmem->buf))[0], ((int*)(shmem->buf))[1], ((int*)(shmem->buf))[2]);

    if (ShMemFree(shmem) != OK)
        ShMemCheck(shmem);

    return 0;
}
