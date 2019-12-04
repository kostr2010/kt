#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

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

#define V_master    if (semop(semid, &addMaster, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define P_master    if (semop(semid, &decMaster, 1) == -1) {\
                       printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define V_queue     if (semop(semid, &addQueue, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define P_queue     if (semop(semid, &decQueue, 1) == -1) {\
                       printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define V_ready     if (semop(semid, &addReady, 1) == -1) {\
                        printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

#define P_ready     if (semop(semid, &decReady, 1) == -1) {\
                       printf("[client] can't increase semaphore's value!\n");\
                        exit(-1);\
                    }

//####################//

void LeaveFullStation();
void WaitForTurn();
void GetToStation();
void PayForJob();
void LeaveService();
void LeaveStation();

//####################//

int main() {
    enum semaphores {
        queue,
        master,
        ready,
        mutex,
    };

    const char* pathname = "master.c";
    const int carsMax = 10;

    struct sembuf   decQueue = {0, -1, 0}, addQueue = {0, 1, 0},
                    decMaster = {1, -1, 0}, addMaster = {1, 1, 0},
                    decReady = {2, -1, 0}, addReady = {2, 1, 0},
                    decMutex = {3, -1, 0}, addMutex = {3, 1, 0};

    ShMem* shmem = ShMemAlloc();

    if (ShMemInit(shmem, sizeof(int), pathname) == -1) {
        printf("[client] unable to create shared memory!\n");
        ShMemCheck(shmem);
        exit(-1);
    } else if (shmem->new == 1) {
        *(int*)(shmem->buf) = carsMax;
        printf("[client] shared memory chunk created!\n vacant places: %d", carsMax);
    }

    // 0 - queue    | for client's queue
    // 1 - master   | for master
    // 2 - ready    | for client, waiting for the end of his service
    // 3 - mutex    | for critical areas
    int semid = semget(shmem->key, 4, 0666 | IPC_CREAT | IPC_EXCL);
    if (semid == -1 && errno == EEXIST) {           // can't open existing semaphore
        semid = semget(shmem->key, 4, 0666);        // try to open existing without additional flags
        if (semid == -1) {
            printf("[client] can't get semaphore!\n");
            exit(-1);
        }
    } else if (semid == -1 && errno != EEXIST) {    // can't create semaphore
        printf("[client] can't get semaphore!\n");
        exit(-1);
    } else {                                        // successfully created semaphore
        semctl(semid, queue, SETVAL, 1);
        semctl(semid, master, SETVAL, 0);
        semctl(semid, ready, SETVAL, 0);
        semctl(semid, mutex, SETVAL, 1);
    }

    P_mutex;

    if (*(int*)(shmem->buf) == 0) {

       V_mutex;

        LeaveFullStation();
    } else {
        *(int*)(shmem->buf) -= 1;

        V_mutex;       

        WaitForTurn();

        P_queue;

        P_mutex;

        *(int*)(shmem->buf) += 1; 

        V_mutex;

        GetToStation(semid);

        V_master;

        PayForJob();

        P_ready;    

        LeaveService();

        V_queue;

        LeaveStation();
    }

    ShMemFree(shmem);

    return 0;
}

//####################//

void LeaveFullStation() {
    printf("[client] station is full, leaving...\n");

    return;
}

void WaitForTurn() {
    printf("[client] waiting for my turn...\n");

    return;
}

void GetToStation() {
    printf("[client] left my transport in the service\n");

    return;
}

void PayForJob() {
    printf("[client] paid the master for his job, now waiting until car will be repaired...\n");

    return;
}

void LeaveService() {
    printf("[client] left the stand\n");

    return;
}

void LeaveStation() {
    printf("[client] left the service\n");

    return;
}
