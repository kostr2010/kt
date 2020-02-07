#ifndef SH_MEM_INCLUDED
#define SH_MEM_INCLUDED

//####################//

#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//####################//

enum ShMemErrs {
    OK,
    E_INIT_ERR,
    E_ATTACH_ERR,
    E_DETACH_ERR,
    E_GET_KEY_ERR,
    E_CANT_ALLOC_SHMEM,
    E_CANT_FIND_SHMEM,
};

struct _ShMem {
    key_t key;
    int shmid;
    char* buf;
    int size;
    enum ShMemErrs err;
    char new;
};
typedef struct _ShMem ShMem;

//####################//

ShMem* ShMemAlloc();
int ShMemInit(ShMem* shmem, const int size, const char* pathname);
int ShMemFree(ShMem* shmem);
void ShMemClear(ShMem* shmem);
void ShMemCheck(ShMem* shmem);

//####################//

#endif