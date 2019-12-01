//#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/msg.h>
//#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "msg.h"

//####################//

const char* pathname = "task2-a.c";
const int msgLast = 17;

//####################//

int main() {
    key_t key = ftok(pathname, 0);

    MyMsgNum* msgn = MyMsgNumAlloc();

    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        printf("can't get message queue!\n");
        exit(-1);
    }

    while (1) {
        if (msgrcv(msgid, (struct msgbuf *) (msgn), sizeof(DataNum), 0, 0) == -1){
            printf("Can\'t receive message from queue\n");
            msgctl(msgid, IPC_RMID, NULL);
            exit(-1);
        }
        
        if (msgn->mtype == msgLast) {
            msgctl(msgid, IPC_RMID, NULL);
            break;
        }

        printf("message type = %ld, info = %d %f\n", msgn->mtype, (msgn->mdata).msgI, (msgn->mdata).msgF);
    }

    return 0;
}

//####################//

