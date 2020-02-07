#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int fdParCh[2], fdChPar[2], result;

    size_t size;
    char  resstring[14];

    if(pipe(fdParCh) < 0 || pipe(fdChPar) < 0){
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    result = fork();

    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {

        /* Parent process */

        close(fdParCh[0]);  // closing read channel
        close(fdChPar[1]);  // closing write channel

        size = write(fdParCh[1], "Hello, child!", 14);
        printf("child said: <Hello, child!>\n");

        if(size != 14){
          printf("Can\'t write all string to pipe\n");
          exit(-1);
        }

        char* buf1 = calloc(15, sizeof(char));

        read(fdChPar[0], buf1, 14);

        printf("parent recieved: <%s>\n");

        close(fdParCh[1]);
        close(fdChPar[0]);

        printf("Parent exit\n");

        free(buf1);
    } else {

        /* Child process */

        close(fdParCh[1]);  // closing write channel
        close(fdChPar[0]);  // closing read channel

        size = write(fdChPar[1], "Hello, daddy!", 14);
        printf("child said: <Hello, daddy!>\n");

        if(size != 14){
          printf("Can\'t write all string to pipe\n");
          exit(-1);
        }

        char* buf2 = calloc(15, sizeof(char));

        read(fdParCh[0], buf2, 14);

        printf("child recieved: <%s>\n");

        close(fdParCh[1]);
        close(fdChPar[0]);

        printf("Child exit\n");

        free(buf2);
    }

    return 0;
}   
