#include "shmem.h"

//####################//

const char* msg = "hail hydra";

const char* shMemErrDesc[] = {
    "ok\n",
    "unable to allocate memory for ShMem structure!\n",
    "can't attach shared memory!\n",
    "can't detach shared memory!\n",
    "can't get key!\n",
    "can't allocate shared memory!\n",
    "can't find shared memory!\n",
};

//####################//

ShMem* ShMemAlloc() {
    ShMem* shmem = calloc(1, sizeof(ShMem));

    return shmem;
}

int ShMemInit(ShMem* shmem, const int size, const char* pathname) {
    shmem->err = OK;
    shmem->size = size;
    shmem->new = 1;

    shmem->key = ftok(pathname, 0);
    if (shmem->key == -1) {
        shmem->err = E_GET_KEY_ERR;
        return shmem->err;
    }

    shmem->shmid = shmget(shmem->key, size, IPC_CREAT | IPC_EXCL | 0666);
    if (shmem->shmid == -1) {
        if (errno != EEXIST) {
            shmem->err = E_CANT_ALLOC_SHMEM;
            return shmem->err;
        } else if ((shmem->shmid = shmget(shmem->key, shmem->size, 0666)) == -1) {
            shmem->err = E_CANT_FIND_SHMEM;
            return shmem->err;
        } else {
            shmem->new = 0;
        }
    }

    shmem->buf = (char*)(shmat(shmem->shmid, NULL, 0));
    if (shmem->buf == (char*)(-1)) {
        shmem->err = E_ATTACH_ERR;
        return shmem->err;
    }

    return shmem->err;
}

int ShMemFree(ShMem* shmem) {
    if (shmdt(shmem->buf) == -1) {
        shmem->err = E_DETACH_ERR;
        return shmem->err;
    }

    return shmem->err;
}

void ShMemClear(ShMem* shmem) {
    memset(shmem->buf, '\0', shmem->size);
}

void ShMemCheck(ShMem* shmem) {
    if (shmem->err != OK) {
        printf("ERROR!\n"
               "%s", shMemErrDesc[shmem->err]);
        exit(-1);
    }
}