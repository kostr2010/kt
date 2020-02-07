#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//####################//

int res = 0;
const int thrNum = 10;
pthread_mutex_t mutex;

//####################//

void* StartRoutine(void * arg); // start routine for pthread_create
//void* StartRoutine(void * arg);

//####################//

int main() {
    pthread_t thrMain = pthread_self();
    pthread_t threads[thrNum];

    pthread_mutex_init(&mutex, NULL);
    
    for (int i = 0; i < thrNum; i++) {
        if (pthread_create(&threads[i], NULL, &StartRoutine, (void*)(&i)) != 0) {
            printf("unable to create thread %d!\n", i);
            exit(-1);
        }
        // how to make them execute in order without busy-wait situation?
    }
    
    for (int i = 0; i < thrNum; i++)
        pthread_join(threads[i], NULL);

    res++;

    printf("terminated thread * %lu, res = %d\n", thrMain, res);

    pthread_mutex_destroy(&mutex);

    return 0;
}

//####################//

void* StartRoutine(void* arg) {
    pthread_t thr = pthread_self();
    pthread_mutex_lock(&mutex);
        res++;
        printf("terminated thread %lu, it was created %d-th, res = %d\n", thr, *(int*)(arg), res);
    pthread_mutex_unlock(&mutex);
    
    pthread_exit(NULL);
}