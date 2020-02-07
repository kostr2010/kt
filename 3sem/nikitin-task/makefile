CC = gcc
CFLAGS = -I. -Wall

all: client master clear

client: shmem.o shmem.h client.o
	$(CC) -o client client.o shmem.o 

master: shmem.o shmem.h master.o
	$(CC) -o master master.o shmem.o

clear:
	rm -rf shmem.o master.o client.o