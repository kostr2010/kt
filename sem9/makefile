CC = gcc
CFLAGS = -I. -Wall

all: 91a 91b task2-a task2-b task3-a task3-b clear

91a: 09-1a.o
	$(CC) -o 91a 09-1a.o

91b: 09-1b.o
	$(CC) -o 91b 09-1b.o

task2-a: task2-a.o msg.o msg.h
	$(CC) -o t2a task2-a.o msg.o

task2-b: task2-b.o msg.o msg.h
	$(CC) -o t2b task2-b.o msg.o

task3-a: task3-a.o msg.o msg.h
	$(CC) -o t3a task3-a.o msg.o

task3-b: task3-b.o msg.o msg.h
	$(CC) -o t3b task3-b.o msg.o

clear:
	rm -rf 09-1a.o 09-1b.o
	rm -rf task2-a.o task2-b.o
	rm -rf task3-a.o task3-b.o