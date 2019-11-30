#include "shmem.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    const char* pathname = "task4-a.c";

    ShMem* shmem = ShMemAlloc();

    if (ShMemInit(shmem, 6 * sizeof(int), pathname) != OK)
        ShMemCheck(shmem);
        
    if(shmem->new){
        ((int*)(shmem->buf))[0] = 0;    // counter a
        ((int*)(shmem->buf))[1] = 1;    // counter b
        ((int*)(shmem->buf))[2] = 1;    // counter all
        ((int*)(shmem->buf))[3] = 0;    // flag[0]
        ((int*)(shmem->buf))[4] = 0;    // flag[1]
        ((int*)(shmem->buf))[5] = 0;    // turn
    } else {    // critical area
        ((int*)(shmem->buf))[4] = 1;    // process b wants to enter shmem
        ((int*)(shmem->buf))[5] = 0;    // set turn to process a
        while (((int*)(shmem->buf))[3] == 1 && ((int*)(shmem->buf))[5] == 0) {
            continue;
        }

        ((int*)(shmem->buf))[1] += 1;
        sleep(5);
        ((int*)(shmem->buf))[2] += 1;

        ((int*)(shmem->buf))[4] = 0;    // process b doesn't want to enter shmem
    }

    printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
           ((int*)(shmem->buf))[0], ((int*)(shmem->buf))[1], ((int*)(shmem->buf))[2]);

    if (ShMemFree(shmem) != OK)
        ShMemCheck(shmem);

    return 0;
}
