#ifndef SEM_H_INCLUDED
#define SEM_H_INCLUDED

//####################//

#include <sys/types.h>
#include <sys/ipc.h>

//####################//

enum Flags {
    NOFLAG,
    CREAT,
    NOWAIT,
};

struct _MyMsg {
    long mtype;
    int data;
};
typedef struct _MyMsg MyMsg;

struct _Semop {
    int op;
    int flag;
};
typedef struct _Semop Semop;

//####################//

int _semget(key_t key, const int flag);
int _semgetval(const int semid);
int _semop(const int semid, Semop* sop);

//####################//

#endif