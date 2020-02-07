#include <stdio.h>
#include <string.h>
#include "shmem.h"

//####################//

const int MAX_LEN = 4000;

//####################//

int main() {
    ShMem* shmem = ShMemAlloc();

    if (ShMemInit(shmem, MAX_LEN, "task2-writer.c") != OK)
        ShMemCheck(shmem);

    printf("<%s>\n", shmem->buf);

    ShMemClear(shmem);

    if (ShMemFree(shmem) != OK)
        ShMemCheck(shmem);
    
    shmctl(shmem->shmid, IPC_RMID, NULL);

    return 0;
}