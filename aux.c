////////////////////////////////////////////
// Nazarov K 812 group task 2             //
//                                        //
// program looks for print argument.      //
// if found, next argument will be passed // 
// as the argument of auxillary program   //
// called aux.c                           //
////////////////////////////////////////////


#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    printf("\nexecuting auxillary program:\n");
    printf("%s \n", argv[1]);

    return 0;
}