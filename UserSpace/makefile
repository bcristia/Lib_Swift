CC=gcc
CFLAGS=-g -Wall -fPIC
BIN=bin
SRC=src
LIB=lib

ALL: directory $(LIB)/libswift.so $(BIN)/server $(BIN)/client

directory:
	mkdir -p $(BIN)
	mkdir -p $(LIB)

$(LIB)/libswift.so: $(BIN)/lib_swift.o
	gcc -shared $(BIN)/lib_swift.o -o $(LIB)/libswift.so

$(BIN)/client: $(SRC)/client.c
	gcc -Wall $(SRC)/client.c -o $(BIN)/client -lswift -L$(LIB)

$(BIN)/server: $(SRC)/server.c
	gcc -Wall $(SRC)/server.c -o $(BIN)/server -lswift -L$(LIB)


$(BIN)/lib_swift.o: $(SRC)/lib_swift.c
	$(CC) $(CFLAGS) -o $(BIN)/lib_swift.o -c $(SRC)/lib_swift.c
	
clean:
	rm -fr $(BIN) $(LIB)
