# main
CFLAGS = -g -O2 -std=c99 -Wall -Wconversion -Wno-sign-conversion -Wbad-function-cast -Wshadow -Wpointer-arith -Wunreachable-code -Wformat=2 -Werror
OBJ = o/hash.o o/vector_dinamico.o o/testing.o o/main.o o/nodo_hash.o o/pruebas_catedra.o
EXEC = main
CC = gcc

all:main

o/hash.o:hash.c hash.h
	$(CC) $(CFLAGS) -c -o o/hash.o hash.c

o/vector_dinamico.o:vector_dinamico.c vector_dinamico.h
	$(CC) $(CFLAGS) -c -o o/vector_dinamico.o vector_dinamico.c

o/nodo_hash.o:nodo_hash.c nodo_hash.h
	$(CC) $(CFLAGS) -c -o o/nodo_hash.o nodo_hash.c

o/testing.o:testing.c testing.h
	$(CC) $(CFLAGS) -c -o o/testing.o testing.c

o/pruebas_catedra.o:pruebas_catedra.c
	$(CC) $(CFLAGS) -c -o o/pruebas_catedra.o pruebas_catedra.c

o/main.o:main.c
	$(CC) $(CFLAGS) -c -o o/main.o main.c

$(EXEC):$(OBJ)
	$(CC) -o main $(OBJ)

.PHONY: all clean count
