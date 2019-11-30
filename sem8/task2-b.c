#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

//####################//

const char* path = "task2-a.c";

//####################//

int main() {    
    key_t key = ftok(path, 0);
    struct sembuf add = {0, 1, 0};

    int semid = semget(key, 1, 0666 | IPC_CREAT);
    if (semid == -1) {
        printf("can't get the semaphore!\n");
        exit(-1);
    }

    if (semop(semid, &add, 1) == -1) {
        printf("can't dicrease semaphore's value!\n");
        exit(-1);
    }

    printf("semaphore's value is %d now. terminated successfully\n", semctl(semid, 0, GETVAL));

    return 0;
}


