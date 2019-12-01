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
const int nmsg = 3;
const int msgLastNum = 19;
const int msgLastText = 17;

//####################//

int main() {
    key_t key = ftok(pathname, 0);
    MyMsgText* msgt = MyMsgTextAlloc();
    if (MyMsgTextInit(msgt, 2, "hullo, yullo")) {
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
    
    while (1) {
        if (msgrcv(msgid, (struct msgbuf *) (msgn), sizeof(DataNum), 1, 0) == -1){
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        
        if (msgn->mtype == msgLastNum) {
            msgctl(msgid, IPC_RMID, NULL);
            break;
        }

        printf("message type = %ld, info = %d %f\n", msgn->mtype, (msgn->mdata).msgI, (msgn->mdata).msgF);
    }

    return 0;
}

//####################//

