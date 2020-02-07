#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int fildes[2], result;

    if(pipe(fildes) < 0){
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    result = fork();

    if(result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {
        /* Parent process */

        close(fildes[0]);

        char str[10] = {};

        sprintf(str, "%d", fildes[1]);

        execl("./writer", "./writer", str, NULL);

    } else {
        /* Child process */

        close(fildes[1]);

        char str[10] = {};

        sprintf(str, "%d", fildes[0]);

        execl("./reader", "./reader", str, NULL);

        close(fildes[0]);
    }

    return 0;
}   
