CC = gcc
CFLAGS = -I. -Wall

all: 81a 81b task2-a task2-b task3-a task3-b task4 clear

81a: 08-1a.o
	$(CC) -o 81a 08-1a.o

81b: 08-1b.o
	$(CC) -o 81b 08-1b.o

task2-a: task2-a.o
	$(CC) -o t2a task2-a.o

task2-b: task2-b.o
	$(CC) -o t2b task2-b.o

task3-a: task3-a.o shmem.o
	$(CC) -o t3a task3-a.o shmem.o

task3-b: task3-b.o shmem.o
	$(CC) -o t3b task3-b.o shmem.o

task4: task4.o
	$(CC) -o t4 task4.o 

clear:
	rm -rf 08-1a.o 08-1b.o
	rm -rf task2-a.o task2-b.o
	rm -rf task3-a.o task3-b.o shmem.o
	rm -rf task4.o
