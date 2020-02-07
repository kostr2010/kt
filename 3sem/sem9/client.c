#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>

#include "msg.h"

//####################//

int main() {
    char* str = NULL;
    char end = 0;
    const int pid = getpid();
    const char* pathname = "server.c";
    
    key_t key = ftok(pathname, 0);
    if (key == -1) {
        printf("[client pid: <%d>] unable to obtain server's key!\n", pid);
        exit(-1);
    }

    int msgid = msgget(key, IPC_CREAT);
    if (msgid == -1) {
        printf("[client pid: <%d>] unable to open/crate messge queue!\n", pid);
        exit(-1);
    }
    
    MyMsgText* msgout = MyMsgTextAlloc();
    int res = MyMsgTextInit(msgout, 1, "\0");
    if (res == -1) {
        printf("[client pid: <%d>] unable to initialize message structure!\n", pid);
        exit(-1);
    }

    MyMsgText* msgin = MyMsgTextAlloc();
    res = MyMsgTextInit(msgin, pid, "\0");
    if (res == -1) {
        printf("[client pid: <%d>] unable to initialize message structure!\n", pid);
        exit(-1);
    }

    printf("welcome to the chat!\n"
           "  * your messages can be no longer than 45 symbols\n"
           "  * to quit chatting just type <end>\n");

    while (end == 0) {
        *(int*)(msgout->mbuf) = pid;
        
        str = calloc(50 - sizeof(int), sizeof(char));

        printf("[type your msg...]\n");

        int wrote = read(fileno(stdin), str, 50 - sizeof(int) + 1);
        if (wrote == -1) {
            printf("[client pid: <%d>] unable to read message for serber from stdin!\n", pid);
            exit(-1);
        } else if (wrote >= 50 - sizeof(int)) {
            printf("[client pid: <%d>] message is too long!\n", pid);
            end = 1;
            continue;
        }

        if (strcmp(str, "end\n") == 0) {
            printf("[client pid: <%d>] end sequence was read. closing terminal...\n", pid);
            free(str);
            end = 1;
            continue;
        }

        memcpy(msgout->mbuf + sizeof(int), str, 50 - sizeof(int));
        memset(str, '\0', 50 - sizeof(int));

        printf("[client pid: <%d>] now sending message to server...\n", pid);
        if (msgsnd(msgid, msgout, 50, 0) == -1) {
            printf("[client pid: <%d>] unable to send message to server!\n", pid);
            exit(-1);
        }

        printf("[client pid: <%d>] waiting for response...\n", pid);
        if (msgrcv(msgid, msgin, 50, pid, 0) == -1) {
            printf("[client pid: <%d>] couldn't recieve server message!\n", pid);
            exit(-1);
        }

        char* pos = strchr(msgin->mbuf,'\n');
        if (pos != NULL)
            *pos = '\0';

        printf("[client pid: <%d>] server responce: <%s>\n", pid, msgin->mbuf);
    }

    MyMsgTextFree(msgin);

    MyMsgTextFree(msgout);

    return 0;
}
