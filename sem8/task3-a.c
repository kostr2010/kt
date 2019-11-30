#include <stdio.h>
#include "shmem.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

//####################//

const char* pathname = "task3-a.c";

//####################//

int main() {
    struct sembuf sub = {0, -1, 0}, add = {0, 1, 0};
    
    ShMem* shmem = ShMemAlloc();

    if (ShMemInit(shmem, 6 * sizeof(int), pathname) != OK) // 3 for counters, 4 & 5 for flag, 6 for turn
        ShMemCheck(shmem);

    int semid = semget(shmem->key, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (semid == -1 && errno == EEXIST) {
        semid = semget(shmem->key, 1, 0666);
        if (semid == -1) {
            printf("can't get semaphore!\n");
            exit(-1);
        }
    } else if (semid == -1 && errno != EEXIST) {
        printf("can't get semaphore!\n");
        exit(-1);
    } else
        semctl(semid, 0, SETVAL, 1);

    if(shmem->new) {
        ((int*)(shmem->buf))[0] = 1;    // counter a
        ((int*)(shmem->buf))[1] = 0;    // counter b
        ((int*)(shmem->buf))[2] = 1;    // counter all
    } else {    // critical area
        if (semop(semid, &sub, 1) == -1) {
        printf("can't dicrease semaphore's value!\n");
        exit(-1);
        }

        ((int*)(shmem->buf))[0] += 1;
        printf("sleeping...\n");
        sleep(3);
        ((int*)(shmem->buf))[2] += 1;

        if (semop(semid, &add, 1) == -1) {
        printf("can't increase semaphore's value!\n");
        exit(-1);
        }
    }

    printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
           ((int*)(shmem->buf))[0], ((int*)(shmem->buf))[1], ((int*)(shmem->buf))[2]);

    if (ShMemFree(shmem) != OK)
        ShMemCheck(shmem);

    return 0;
}
