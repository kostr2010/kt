#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sem.h"

//####################//

int main() {
    key_t key = ftok("sem.c", 0);

    int semid = _semget(key, CREAT);
    if (semid == -1) {
        printf("not initialized\n");
    }  

    Semop add = {1, 0}, dec = {-4, 0};
    _semop(semid, &add);

    //sleep(1);

    _semop(semid, &dec);

    //sleep(3);

    _semop(semid, &add);

    //msgctl(semid, IPC_RMID, NULL);

    return 0;
}
