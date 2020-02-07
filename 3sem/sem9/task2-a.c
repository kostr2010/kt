//#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/msg.h>
//#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "msg.h"

//####################//

const char* pathname = "task2-a.c";
const int nmsg = 7;
const int msgLast = 2;

//####################//

int main() {
    key_t key = ftok(pathname, 0);

    MyMsgNum* msgn = MyMsgNumAlloc();
    if (MyMsgNumInit(msgn, 1, 123, 0.456)) {
        printf("initialization error!\n");
        exit(-1);
    }

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

    msgn->mtype = msgLast;

    if (msgsnd(msgid, (void*)(msgn), 0, 0) == -1) {
        printf("Can\'t send message to queue\n");
        msgctl(msgid, IPC_RMID, NULL);
        exit(-1);
    }

    printf("%d messages were sent successfully!\n", nmsg);

    return 0;
}

//####################//

