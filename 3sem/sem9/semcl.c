#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sem.h"

//####################//

int main() {
    key_t key = ftok("sem.c", 0);

    int semid = _semget(key, CREAT);
    if (semid == -1) {
        printf("not initialized!\n");
    }  

    Semop add = {4, 0}, dec = {-1, 0};

    _semop(semid, &dec);

    //sleep(1);

    _semop(semid, &add);

    //_sleep(3);

    _semop(semid, &dec);
    //msgctl(semid, IPC_RMID, NULL);

    return 0;
}
