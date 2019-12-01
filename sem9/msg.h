#ifndef MSG_H_INCLUDED
#define MSG_H_INCLUDED

//####################//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//####################//

struct _DataText {
    char* buf;
    int maxLen;
};
typedef struct _DataText DataText;

struct _DataNum {
    int msgI;
    float msgF;
};
typedef struct _DataNum DataNum;

struct _MyMsgNum {
    long mtype;
    DataNum mdata;
};
typedef struct _MyMsgNum MyMsgNum;

struct _MyMsgText {
    long mtype;
    char mbuf[50];
};
typedef struct _MyMsgText MyMsgText;

//####################//

MyMsgNum* MyMsgNumAlloc();
int MyMsgNumInit(MyMsgNum* msg, const long mtype, const int dataI, const float dataF);
void MyMsgNumFree(MyMsgNum* msg);

MyMsgText* MyMsgTextAlloc();
int MyMsgTextInit(MyMsgText* msg, const long mtype, const char* text);
void MyMsgTextFree(MyMsgText* msg);

//####################//

#endif