CC=gcc
CFLAGS=-g -Wall -fPIC -DDEBUG

BIN=bin
SRC=src
LIB=lib

ALL: directory $(LIB)/libswiftraw.so $(BIN)/server $(BIN)/client

directory:
	mkdir -p $(BIN)
	mkdir -p $(LIB)

$(LIB)/libswiftraw.so: $(BIN)/lib_swift_raw.o
	gcc -shared $(BIN)/lib_swift_raw.o -o $(LIB)/libswiftraw.so

$(BIN)/client: $(SRC)/client.c
	$(CC) $(CFLAGS) -o $@ $^ -lswiftraw -L$(LIB)

$(BIN)/server: $(SRC)/server.c
	$(CC) $(CFLAGS) -o $@ $^ -lswiftraw -L$(LIB)

$(BIN)/lib_swift_raw.o: $(SRC)/lib_swift_raw.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm -fr $(BIN) $(LIB)

