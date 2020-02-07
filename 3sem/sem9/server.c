#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <pthread.h>
#include <error.h>
#include <errno.h>

#include "msg.h"

//####################//

struct _ThreadArgs {
    MyMsgText msgin;
    int msgid;
    int semid;
};
typedef struct _ThreadArgs ThreadArgs;

//####################//

void* StartRoutine(void* args);

//####################//

int main() {
    pthread_t thread = 0;

    struct sembuf dec = {0, -1, 0};

    const char* pathname = "server.c";
    
    key_t key = ftok(pathname, 0);
    if (key == -1) {
        printf("[server] unable to obtain server's key!\n");
        exit(-1);
    }

    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) {
        printf("[server] unable to open/crate messge queue!\n");
        exit(-1);
    }

    MyMsgText* msgin = MyMsgTextAlloc();
    int res = MyMsgTextInit(msgin, 1, "\0");
    if (res == -1) {
        printf("[server] unable to initialize message structure!\n");
        exit(-1);
    }

    int semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (semid == -1 && errno == EEXIST) {           // can't open existing semaphore
        semid = semget(key, 1, 0666);        // try to open existing without additional flags
        if (semid == -1) {
            printf("[master] can't get semaphore!\n");
            exit(-1);
        }
    } else if (semid == -1 && errno != EEXIST) {    // can't create semaphore
        printf("[master] can't get semaphore!\n");
        exit(-1);
    } else {                                        // successfully created semaphore
        semctl(semid, 0, SETVAL, 1);
    }

    while(1) {
        if (msgrcv(msgid, (struct msgbuf *)(msgin), 50, 1, 0) == -1) {
            printf("[server] can't recieve messages!\n");
            perror("message: \n");
            exit(-1);
        }
        //mutex
        if (semop(semid, &dec, 1) == -1) {
            printf("[client] can't increase semaphore's value!\n");
            exit(-1);
        }

        ThreadArgs args = {*msgin, msgid, semid};

        if (pthread_create(&thread, NULL, &StartRoutine, (void*)(&args)) != 0) {
            printf("[server] unable to create thread!\n");
            exit(-1);
        }
    }

    return 0;
}

//####################//

void* StartRoutine(void* args) {
    ThreadArgs* cpy = calloc(1, sizeof(ThreadArgs));
    memcpy(cpy, (ThreadArgs*)(args), sizeof(ThreadArgs));
    
    struct sembuf inc = {0, 1, 0};
    if (semop(cpy->semid, &inc, 1) == -1) {\
        printf("[client] can't increase semaphore's value!\n");\
        exit(-1);\
    }

    int mtype = *(int*)(cpy->msgin.mbuf);

    printf("[subserver] processing %d's message...\n", mtype);
    sleep(5);

    MyMsgText* msgout = MyMsgTextAlloc();

    int res = MyMsgTextInit(msgout, mtype, cpy->msgin.mbuf + sizeof(int));
    if (res == -1) {
        printf("[subserver] unable to initialize message structure!\n");
        exit(-1);
    }

    if (msgsnd(cpy->msgid, msgout, 50, IPC_NOWAIT) == -1) {
        if (errno == EAGAIN) {
            printf("[subserver] can't send messages! client is offline!\n");
            exit(-1);
        } else {
            printf("[subserver] can't send messages!\n");
            exit(-1);
        }
    }

    printf("[subserver] done with %d\n", mtype);

    MyMsgTextFree(msgout);
    free(cpy);

    return NULL;
}
