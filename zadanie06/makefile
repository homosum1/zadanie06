
# kompilator
CC = gcc

#flagi
CFLAGS = -Wall -O0 -std=c11 -pthread -pedantic


all: semafory_b powielacz

powielacz : powielacz.c my_lib.h
	$(CC) $(CFLAGS) -o powielacz powielacz.c

semafory_b : main.c my_lib.h
	$(CC) $(CFLAGS) -o semafory_b main.c
