CC=gcc
CFLAGS=-g -Wall -fPIC -DDEBUG

BIN=bin
SRC=src
LIB=lib

ALL: directory  $(BIN)/server $(BIN)/client

directory:
	mkdir -p $(BIN)

$(BIN)/client: $(SRC)/client.c
	$(CC) $(CFLAGS) -o $@ $^

$(BIN)/server: $(SRC)/server.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -fr $(BIN)

