#include <stdio.h>
#include <string.h>
#include "shmem.h"

//####################//

int main() {
    ShMem* shmem = ShMemAlloc();

    if (ShMemInit(shmem, strlen("hail hydra"), "task2-writer.c") != OK)
        ShMemCheck(shmem);

    printf("<%s>\n", shmem->buf);

    ShMemClear(shmem);

    if (ShMemFree(shmem) != OK)
        ShMemCheck(shmem);
}