#include <stdio.h>
#include <string.h>
#include "shmem.h"

//####################//

int main() {
    const char* msg = "hail hydra";

    ShMem* shmem = ShMemAlloc();

    if (ShMemInit(shmem, strlen(msg), "task2-writer.c") != OK)
        ShMemCheck(shmem);

    memcpy(shmem->buf, msg, strlen(msg));

    if (ShMemFree(shmem) != OK)
        ShMemCheck(shmem);
}


