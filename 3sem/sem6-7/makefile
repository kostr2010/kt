CC = gcc
CFLAGS = -I. -Wall

all: 61a 61b 62 task2-reader task2-writer 63a 63b task4-a task4-b task5 clear

61a: 06-1a.o
	$(CC) -o 61a 06-1a.o

61b: 06-1b.o
	$(CC) -o 61b 06-1b.o

62: 06-2.o
	$(CC) -o 62 06-2.o -pthread 

63a: 06-3a.o
	$(CC) -o 63a 06-3a.o

63b: 06-3b.o
	$(CC) -o 63b 06-3b.o

task2-writer: shmem.o shmem.h task2-writer.o 
	$(CC) -o t2-writer task2-writer.o shmem.o

task2-reader: shmem.o shmem.h task2-reader.o
	$(CC) -o t2-reader task2-reader.o shmem.o

task4-a: shmem.o task4-a.o shmem.h
	$(CC) -o t4-a task4-a.o shmem.o

task4-b: shmem.o task4-b.o shmem.h
	$(CC) -o t4-b task4-b.o shmem.o

task5: task5.o
	$(CC) -o t5 task5.o -pthread

clear:
	rm -rf 06-1a.o 06-1b.o 06-2.o 06-3a.o 06-3b.o
	rm -rf task2-reader.o shmem.o task2-writer.o
	rm -rf task4-a.o task4-b.o
	rm -rf task5.o