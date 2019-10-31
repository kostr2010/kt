#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

int main() {

	char name[] = "aaa.fifo";
	int fd = 0;
	if((fd = open(name, O_RDONLY)) < 0){
		printf("Can\'t open FIFO for reading\n");
		exit(-1);
	}
	char str[20] = {};

	int size = read(fd, str, 14);
	if(size != 14){
		printf("Can't read all string\n");
		exit(-2);
	}
	printf("string: %s\n", str);
	close(fd);
	return 0;
}

