CC=gcc
CFLAGS=-g -Wall -fPIC
BIN=bin
SRC=src
LIB=lib

ALL: directory lib main

directory:
	mkdir -p $(BIN)
	mkdir -p $(LIB)

lib: object
	gcc -shared $(BIN)/lib_swift.o -o $(LIB)/libswift.so

main:
	gcc -Wall $(SRC)/main.c -o $(BIN)/main -lswift -L$(LIB)

object: $(SRC)/lib_swift.c
	$(CC) $(CFLAGS) -o $(BIN)/lib_swift.o -c $(SRC)/lib_swift.c
	
clean:
	rm -fr $(BIN) $(LIB)
