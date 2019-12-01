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
const int msgLastNum = 19;
const int msgLastText = 17;

//####################//

int main() {
    key_t key = ftok(pathname, 0);

    MyMsgNum* msgn = MyMsgNumAlloc();
    if (MyMsgNumInit(msgn, 1, 123, 0.456)) {
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

    while (1) {
        if (msgrcv(msgid, (struct msgbuf *) (msgt), 50, 2, 0) == -1){
            printf("Can\'t receive message from queue\n");
            msgctl(msgid, IPC_RMID, NULL);
            exit(-1);
        }

        if (msgt->mtype == msgLastText) {
            msgctl(msgid, IPC_RMID, NULL);
            break;
        }

        printf("message type = %ld, info = <%s>\n", msgt->mtype, msgt->mbuf);
    }

    return 0;
}

//####################//

