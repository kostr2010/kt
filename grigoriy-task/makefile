CC = gcc
CFLAGS = -I. -Wall

all: boy girl clear

boy: boy.o shmem.o shmem.h
	$(CC) -o boy boy.o shmem.o

girl: girl.o shmem.o shmem.h
	$(CC) -o girl girl.o shmem.o

clear:
	rm -rf girl.o boy.o shmem.o