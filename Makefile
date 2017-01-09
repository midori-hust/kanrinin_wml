CC = gcc
CFLAGS = -g -std=c99

all: main

main: main.o nyuukai.o keisoku.o sakujyo.o
	$(CC) main.o nyuukai.o keisoku.o sakujyo.o -o main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

nyuukai.o: nyuukai.c nyuukai.h
	$(CC) $(CFLAGS) -c nyuukai.c -o nyuukai.o

sakujyo.o: sakujyo.c sakujyo.h
	$(CC) $(CFLAGS) -c sakujyo.c -o sakujyo.o

keisoku.o: keisoku.c keisoku.h
	$(CC) $(CFLAGS) -c keisoku.c -o keisoku.o

clean:
	rm -rf *.o *~ main *.tbl
