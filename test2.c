////////////////////////////////////////////
// Nazarov K 812 group task 3             //
//                                        //
// program creates file with 0600 mode,   //
// writes down the line, then prints it   //
// in terminal                            //
////////////////////////////////////////////

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    int     fd;
    size_t  size;
    char    string[] = "Hello, user!";

    (void)umask(0);

    if((fd = open("FILE.txt", O_RDWR | O_CREAT | O_TRUNC, 0600)) < 0){
      printf("Can\'t open file\n");
      exit(-1);
    }

    size = write(fd, string, 12);

    if(size != 12){
      printf("Can\'t write all string\n");
      exit(-1);
    }

    char buf[20] = {};

    lseek(fd, 0, SEEK_SET);

    size = read(fd, buf, 12);

    if(size != 12){
      printf("Can\'t read all string\n");
      exit(-1);
    }

    printf("%s\n", buf);

    if(close(fd) < 0){
      printf("Can\'t close file\n");
    }

    return 0;
}
