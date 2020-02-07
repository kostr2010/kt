#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <error.h>

#include "sem.h"

//####################//

// flags - CREAT or NOFLAG
int _semget(key_t key, const int flag) {    
    int msgid = 0;

    switch (flag) {
        case NOFLAG: 
            msgid = msgget(key, 0666); 
            break;
        case CREAT: 
            if ((msgid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) == -1 && errno == EEXIST) {
                if ((msgid = msgget(key, 0666)) == -1) {
                    printf("[_semget] can't obtain key!\n");
                    exit(-1);
                }
            } else {
                printf("new!\n");
                // V mutex
                MyMsg msg = {1, 1};

                if (msgsnd(msgid, (void*)(&msg), sizeof(int), 0) == -1) {
                    printf("[_semget] cant send mutex message to queue\n");
                    msgctl(msgid, IPC_RMID, NULL);
                    exit(-1);
                }

                // send val == 0
                msg.mtype = 2;
                msg.data = 1;

                if (msgsnd(msgid, (void*)(&msg), sizeof(int), 0) == -1) {
                    printf("[_semget] cant send zero message to queue\n");
                    msgctl(msgid, IPC_RMID, NULL);
                    exit(-1);
                }
            }
            break;
        default: printf("[_semget] invalid flag\n"); break;
    }

    printf("[_semget] semid is: %d\n", msgid);

    return msgid;
}

int _semgetval(const int semid) {
    MyMsg msg = {};
    //msgrcv(semid, &msg, sizeof(int), 1, 0);

    int semval = 0;
    while (!(msgrcv(semid, &msg, sizeof(int), semval + 2, IPC_NOWAIT) == -1 && errno == ENOMSG)) {
        //printf("*\n");
        semval++;
    }

    if (errno != ENOMSG)
        printf("ploho\n");

    // if == -1, error occured & value is not even 0
    semval--;

    //printf("[_semval] current semval is: %d\n", semval);
    
    for (int i = 2; i <= semval + 2; i++) {
        msg.mtype = i;
        msg.data = 1;
        
        if (msgsnd(semid, &msg, sizeof(int), 0) == -1) {
            printf("[_semgetval] can't send back message N0 %d!\n", i);
            exit(-1);
        }
    }

    return semval;
}

int _semop(const int semid, Semop* sop) {
    MyMsg msg = {};
    msgrcv(semid, &msg, sizeof(int), 1, 0);
    printf("enterd semop\n");
    //sleep(2);

    if (sop->op > 0) {
        //printf("9\n");
        int val = _semgetval(semid);

        for (int i = 2 + val + 1; i <= val + sop->op + 2; i++) {
            msg.mtype = i;
            //printf("i %d\n", i);
            msg.data = 1;
            
            if (msgsnd(semid, &msg, sizeof(int), 0) == -1) {
                printf("[_semop] can't send back message N0 %d!\n", i);
                exit(-1);
            }
        }
    } else if (sop->op == 0 && sop->flag == NOFLAG) {
        // resolve busy/wait
    } else if (sop->op == 0 && sop->flag == NOWAIT) {
        if (msgrcv(semid, &msg, sizeof(int), 1, IPC_NOWAIT) == -1 && errno == ENOMSG) {
            return -1;
        } else {
            msg.mtype = 1;
            msg.data = 1;

            if (msgsnd(semid, &msg, sizeof(int), 0) == -1) {
                printf("[_semop] can't send back message N0 %d!\n", 1);
                exit(-1);
            }
        }
    } else if (sop->op < 0 && sop->flag == NOFLAG) {
        int val = _semgetval(semid);

        if (val < -1 * sop->op) {
            msg.data = 1;
            msg.mtype = 1;
            printf("V the mutex, beacuse sop->op is too big\n");
            msgsnd(semid, &msg, sizeof(int), 0);
            printf("trying to recieve message %d\n", 2 - sop->op);
            if (msgrcv(semid, &msg, sizeof(int), 2 - sop->op, 0)) {
                msg.data = 1;
                msg.mtype = 2 - sop->op;
                // semval - 1 here (non-atomic)
                msgsnd(semid, &msg, sizeof(int), 0);
                printf("trying to P mutex\n");
                msgrcv(semid, &msg, sizeof(int), 1, 0);
            }

            msg.data = 1;
            msg.mtype = 2 + sop->op;

            msgsnd(semid, &msg, sizeof(int), 0);
        }

        val = _semgetval(semid);

        printf("after wait, val = %d\n", val);

        for (int i = 2 + val; i > 2 + val + sop->op; i--) {
            if (msgrcv(semid, &msg, sizeof(int), i, 0) == -1) {
                printf("ploho\n");
                exit(-1);
            }
                
        }
    } else if (sop->op < 0 && sop->flag == NOWAIT) {
        int val = _semgetval(semid);

        if (val < -1 * sop->op) {
            printf("NOWAIT && semval < op\n");
            return -1;
        } else {
            val = _semgetval(semid);

            for (int i = 2 + val; i > 2 + val + sop->op; i--) {
                msgrcv(semid, &msg, sizeof(int), i, 0);
            }
        }
    }
    
    printf("[_semop] after semop semval is %d\n", _semgetval(semid));    

    //sleep(5);

    msg.mtype = 1;
    msg.data = 1;
    msgsnd(semid, &msg, sizeof(int), 0);

    return 0;
}