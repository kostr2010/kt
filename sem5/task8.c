#define _GNU_SOURCE

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
	
	int fd[2] = {};
	char str[30] = {};
	pipe(fd);

	printf("%d\n", fcntl(fd[1], F_GETPIPE_SZ));
    fcntl(fd[1], F_SETPIPE_SZ, 5000);
	printf("%ld\n", (long)fcntl(fd[1], F_GETPIPE_SZ));

	close(fd[0]);
	close(fd[1]);
}