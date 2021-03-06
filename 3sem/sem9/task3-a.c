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
const int nmsg = 7;
const int msgNum = 1;
const int msgLastNum = 2;
const int msgText = 3;
const int msgLastText = 4;

//####################//

void RcvErrHandler();
void RcvTextHandler(MyMsgText* msgt);
void RcvLastTextHandler(char* flag);

//####################//

int main() {
    key_t key = ftok(pathname, 0);

    MyMsgNum* msgn = MyMsgNumAlloc();
    if (MyMsgNumInit(msgn, msgNum, 123, 0.456)) {
        printf("initialization error!\n");
        exit(-1);
    }
    MyMsgText* msgt = MyMsgTextAlloc();
    
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        printf("can't get message queue!\n");
        exit(-1);
    }
    
    for (int i = 0; i < nmsg; i++) {
        if (msgsnd(msgid, (void*)(msgn), sizeof(DataNum), 0) == -1) {
            printf("Can\'t send message to queue\n");
            msgctl(msgid, IPC_RMID, NULL);
            exit(-1);
        }
    }

    msgn->mtype = msgLastNum;

    if (msgsnd(msgid, (void*)(msgn), 0, 0) == -1) {
        printf("Can\'t send message to queue\n");
        msgctl(msgid, IPC_RMID, NULL);
        exit(-1);
    }
    
    char transEnd = 0;    
    while (transEnd == 0) {
        if (msgrcv(msgid, (struct msgbuf *) (msgt), 50, msgText, IPC_NOWAIT) == -1) {
            if (errno != ENOMSG)
                RcvErrHandler();
            else {    // queue empty on text messages. looking for the last numeric
                if (msgrcv(msgid, (struct msgbuf *) (msgt), 50, msgLastText, IPC_NOWAIT) == -1) {
                    if (errno != ENOMSG)
                        RcvErrHandler();
                    else
                        continue;
                } else
                    RcvLastTextHandler(&transEnd);
            }   
        } else
            RcvTextHandler(msgt);
    }
    
    return 0;
}

//####################//

void RcvErrHandler() {
    printf("Can\'t receive message from queue\n");
    exit(-1);
}

void RcvTextHandler(MyMsgText* msgt) {
    printf("message type = %ld, info = <%s>\n", msgt->mtype, msgt->mbuf);
}

void RcvLastTextHandler(char* flag) {
    *flag = 1;
}