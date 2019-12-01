#include "msg.h"

//####################//

int msgTextMaxLen = 50;

//####################//

MyMsgNum* MyMsgNumAlloc() {
    MyMsgNum* msg = calloc(1, sizeof(msg));

    return msg;
}

int MyMsgNumInit(MyMsgNum* msg, long mtype, int dataI, float dataF) {
    if (msg == NULL)
        return -1;

    msg->mtype = mtype;
    msg->mdata.msgI = dataI;
    msg->mdata.msgF = dataF;

    return 0;
}

void MyMsgNumFree(MyMsgNum* msg) {
    if (msg == NULL)
        return;
    else
        free(msg);

    return;
}


MyMsgText* MyMsgTextAlloc() {
    MyMsgText* msg = calloc(1, sizeof(MyMsgText));
    //msg->mbuf = calloc(msgTextMaxLen, sizeof(char));

    return msg;
}

int MyMsgTextInit(MyMsgText* msg, long mtype, const char* text) {
    if (msg == NULL)
        return -1;

    msg->mtype = mtype;
    if (msg->mbuf == NULL)
        return -1;
    
    if (strlen(text) > msgTextMaxLen - 1) {
        printf("text too long\n");
        return -1;
    } else
        memcpy(msg->mbuf, text, strlen(text));

    return 0;
}

void MyMsgTextFree(MyMsgText* msg) {
    if (msg == NULL)
        return;
    else {
        if (msg->mbuf == NULL)
            return;
        else
            free(msg->mbuf);
        
        free(msg);
    }

    return;
}

//####################//