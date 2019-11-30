CC = gcc
CFLAGS = -I. -Wall

all: 91a 91b task2-a task2-b clear

91a: 09-1a.o
	$(CC) -o 91a 09-1a.o

91b: 09-1b.o
	$(CC) -o 91b 09-1b.o

task2-a: task2-a.o msg.o msg.h
	$(CC) -o t2a task2-a.o msg.o

task2-b: task2-b.o msg.o msg.h
	$(CC) -o t2b task2-b.o msg.o

clear:
	rm -rf 09-1a.o 09-1b.o
	rm -rf task2-a.o task2-b.o