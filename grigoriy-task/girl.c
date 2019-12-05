#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include "shmem.h"

//####################//

#define V_mutex     if (semop(semid, &addMutex, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define P_mutex     if (semop(semid, &decMutex, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define V_boys      if (semop(semid, &addBoys, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define P_boys      if (semop(semid, &decBoys, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define C_boys      if (semop(semid, &waitBoys, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define V_girls     if (semop(semid, &addGirls, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define P_girls     if (semop(semid, &decGirls, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define C_girls     if (semop(semid, &waitGirls, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define V_vacant    if (semop(semid, &addVacant, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define P_vacant    if (semop(semid, &decVacant, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

//####################//

int main() {
    enum semaphores {
        girls,
        boys,
        vacant,
        mutex,
    };

    const char* pathname = "boy.c";
    const int vacantMax = 10;

    struct sembuf   decGirls = {0, -1, 0}, addGirls = {0, 1, 0}, waitGirls = {0, 0, 0},
                    decBoys = {1, -1, 0}, addBoys = {1, 1, 0}, waitBoys = {1, 0, 0},
                    decVacant = {2, -1, 0}, addVacant = {2, 1, 0},
                    decMutex = {3, -1, 0}, addMutex = {3, 1, 0};

    ShMem* shmem = ShMemAlloc();

    if (ShMemInit(shmem, sizeof(int), pathname) == -1) {
        printf("[girls] unable to create shared memory!\n");
        ShMemCheck(shmem);
        exit(-1);
    } else if (shmem->new == 1) {
        *(int*)(shmem->buf) = vacantMax;
        printf("[girls] shared memory chunk created!\n vacant places: %d\n", vacantMax);
    }

    // 0 - girls    | for girls
    // 1 - boys     | for boys
    // 2 - vacant   | for vacant places
    // 3 - mutex    | for critical areas
    int semid = semget(shmem->key, 4, 0666 | IPC_CREAT | IPC_EXCL);
    if (semid == -1 && errno == EEXIST) {           // can't open existing semaphore
        semid = semget(shmem->key, 4, 0666);        // try to open existing without additional flags
        if (semid == -1) {
            printf("[girls] can't get semaphore!\n");
            exit(-1);
        }
    } else if (semid == -1 && errno != EEXIST) {    // can't create semaphore
        printf("[girls] can't get semaphore!\n");
        exit(-1);
    } else {                                        // successfully created semaphore
        semctl(semid, girls, SETVAL, 0);
        semctl(semid, boys, SETVAL, 0);
        semctl(semid, vacant, SETVAL, vacantMax);
        semctl(semid, mutex, SETVAL, 1);
    }

    printf("[girls] new girl has arrived\nwaiting for boys to leave the shower...\n");
    C_boys;

    P_mutex;
    P_vacant;
    V_girls;
    V_mutex;
    printf("[girls] entered the shower...\n[girls] washing...");

    sleep(5);

    printf("[girls] chatting with friends...\n");

    sleep(5);

    P_mutex;
    V_vacant;
    P_girls;
    V_mutex;

    ShMemFree(shmem);
}