//#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/msg.h>
//#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>

#include "msg.h"

//####################//

const char* pathname = "task3-a.c";
const int nmsg = 5;
const int msgNum = 1;
const int msgLastNum = 2;
const int msgText = 3;
const int msgLastText = 4;

//####################//

void RcvErrHandler();
void RcvNumHandler(MyMsgNum* msgn);
void RcvLastNumHandler(char* flag);

//####################//

int main() {
    key_t key = ftok(pathname, 0);
    MyMsgText* msgt = MyMsgTextAlloc();
    if (MyMsgTextInit(msgt, msgText, "hullo, yullo")) {
        printf("initialization error!\n");
        exit(-1);
    }
    MyMsgNum* msgn = MyMsgNumAlloc();

    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        printf("can't get message queue!\n");
        exit(-1);
    }
    
    for (int i = 0; i < nmsg; i++) {
        if (msgsnd(msgid, (void*)(msgt), 50, 0) == -1) {
            printf("Can\'t send message to queue\n");
            msgctl(msgid, IPC_RMID, NULL);
            exit(-1);
        }
    }

    msgt->mtype = msgLastText;

    if (msgsnd(msgid, (void*)(msgt), 0, 0) == -1) {
        printf("Can\'t send message to queue\n");
        msgctl(msgid, IPC_RMID, NULL);
        exit(-1);
    }
    
    char transEnd = 0;    
    while (transEnd == 0) {
        if (msgrcv(msgid, (struct msgbuf *) (msgn), sizeof(DataNum), msgNum, IPC_NOWAIT) == -1) {
            if (errno != ENOMSG)
                RcvErrHandler();
            else {  // queue empty on numeric messages. looking for the last numeric
                if (msgrcv(msgid, (struct msgbuf *) (msgn), sizeof(DataNum), msgLastNum, IPC_NOWAIT) == -1) {
                    if (errno != ENOMSG)
                        RcvErrHandler();
                    else
                        continue;
                } else
                    RcvLastNumHandler(&transEnd);
            }
        } else
            RcvNumHandler(msgn);
    }
    
    return 0;
}

//####################//

void RcvErrHandler() {
    printf("Can\'t receive message from queue\n");
    exit(-1);
}

void RcvNumHandler(MyMsgNum* msgn) {
    printf("message type = %ld, info = %d %f\n", msgn->mtype, (msgn->mdata).msgI, (msgn->mdata).msgF);
}

void RcvLastNumHandler(char* flag) {
    *flag = 1;
}