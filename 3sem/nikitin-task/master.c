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

void CheckCar();
int ChartCar(int fd, int rest);
char* GetTimeStamp();

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
        printf("[master] unable to create shared memory!\n");
        ShMemCheck(shmem);
        exit(-1);
    } else if (shmem->new == 1) {
        *(int*)(shmem->buf) = carsMax;
        printf("[master] shared memory chunk created!\n vacant places: %d\n", carsMax);
    }

    // 0 - queue    | for client's queue
    // 1 - master   | for master
    // 2 - ready    | for client, waiting for the end of his service
    // 3 - mutex    | for critical areas
    int semid = semget(shmem->key, 4, 0666 | IPC_CREAT | IPC_EXCL);
    if (semid == -1 && errno == EEXIST) {           // can't open existing semaphore
        semid = semget(shmem->key, 4, 0666);        // try to open existing without additional flags
        if (semid == -1) {
            printf("[master] can't get semaphore!\n");
            exit(-1);
        }
    } else if (semid == -1 && errno != EEXIST) {    // can't create semaphore
        printf("[master] can't get semaphore!\n");
        exit(-1);
    } else {                                        // successfully created semaphore
        semctl(semid, queue, SETVAL, 1);
        semctl(semid, master, SETVAL, 0);
        semctl(semid, ready, SETVAL, 0);
        semctl(semid, mutex, SETVAL, 1);
    }

    int logfd = open("data-base/log.log",  O_RDWR | O_TRUNC | O_CREAT, S_IRUSR| S_IWUSR);
    if (logfd == -1) {
        printf("[master] unable to open database!\n");
        return -1;
    }

    printf("[master] opened the service, waiting for customers!\n");

    while (1) {
        P_master;

        CheckCar();

        V_ready;

        if (ChartCar(logfd, carsMax - *(int*)(shmem->buf)) == -1) {
            printf("[master] unable to write to database!\n");
            exit(-1);
        }
    }

    ShMemFree(shmem);

    close(logfd);
}

//####################//

void CheckCar() {
    printf("[master] car repair initialized...\n");

    sleep(5);

    printf("[master] finished\n");
}

int ChartCar(int fd, int rest) {
    char* time = GetTimeStamp();

    if (dprintf(fd, "[%s] car was successfully repaired. %d to go\n", time, rest) < 0) {
        printf("[master] couldn't update database! my pen is lost! urgently closing repair station!\n");
        return -1;
    }

    free(time);

    return 0;
}

char* GetTimeStamp() {
    time_t ltime;
	struct tm result;
	char date[11];
    char hrs[9];

	ltime = time(NULL);
	localtime_r(&ltime, &result);
    strftime(date, 11, "%F", &result);
    strftime(hrs, 9, "%X", &result);
    char* logName = calloc(22, sizeof(char));
    sprintf(logName, "[%s|%s]", date, hrs);

    return logName;
}
