#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>


//####################//

int WriteToPipe(const int* fildes, const char* msg);
int ReadFromPipe(const int* fildes, char* buf, int bufSz);

//####################//

int main() {
    int fildes[2] = {};
    const int bufSz = 100;
    char* buf = calloc(bufSz, sizeof(char));
    const int N = 11;
    const char* path = "task4.c";
    const char* msgSon = "message to the son"; 
    const char* msgPar = "message to the parent";

    struct sembuf sub1 = {0, -1, 0}, add1 = {0, 1, 0}, 
                  sub2 = {0, -2, 0}, add2 = {0, 2, 0},
                  probeZero = {0, 0, 0};

    key_t key = ftok(path, 0);

    int semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (semid == -1 && errno == EEXIST) {
        semid = semget(key, 1, 0666);
        if (semid == -1) {
            printf("can't get semaphore!\n");
            exit(-1);
        }
    } else if (semid == -1 && errno != EEXIST) {
        printf("can't get semaphore!\n");
        exit(-1);
    }

    if (pipe(fildes) == -1) {
        printf("can't create pipe!\n");
        exit(-1);
    }

    switch (fork()) {
    case -1:    
        // error
        printf("can't fork child!\n");

        break;
    case 0:     
        // child process
        for (int i = 0; i < N; i++) {
            if (semop(semid, &sub1, 1) == -1) {
                printf("can't update semaphore");
                exit(-1);
            }

            if (ReadFromPipe(fildes, buf, bufSz) != strlen(msgSon))
                exit(-1);

            printf("child read: <%s>\n", buf);
            memset(buf, '\0', bufSz);

            if (WriteToPipe(fildes, msgPar) == -1)
                exit(-1);

            if (semop(semid, &add2, 1) == -1) {
                printf("can't update semaphore");
                exit(-1);
            }
        }
        break;
    default:    
        // parent
        for (int i = 0; i < N; i++) {
            if (WriteToPipe(fildes, msgSon) == -1)
                exit(-1);
    
            if (semop(semid, &add1, 1) == -1) {
                printf("can't update semaphore");
                exit(-1);
            }

            if (semop(semid, &sub2, 1) == -1) {
                printf("can't update semaphore");
                exit(-1);
            }

            if (ReadFromPipe(fildes, buf, bufSz) != strlen(msgPar))
                exit(-1);
    
            printf("parent read: <%s>\n", buf);
            memset(buf, '\0', bufSz);
        }
        break;
    }

    printf("successfully terminated %d times\n", N);

    close(fildes[0]);
    close(fildes[1]);

    semctl(semid, IPC_RMID, 0);

    return 0;
}

//####################//

int WriteToPipe(const int* fildes, const char* msg) {
    if (write(fildes[1], msg, strlen(msg)) != strlen(msg)) {
        printf("can't write to pipe!\n");
        return -1;
    }

    return 0;
}

int ReadFromPipe(const int* fildes, char* buf, int bufSz) {
    int res = -1;
    res = read(fildes[0], buf, bufSz);

    return res;
}
