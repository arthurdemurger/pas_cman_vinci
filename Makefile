# === Alias ===
CC = gcc
CFLAGS = -std=c17 -pedantic -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable -D_DEFAULT_SOURCE

# === Fichiers ===
UTILS_OBJ = build/utils_v3.o
SERVER_OBJ = build/pas_server.o
CLIENT_OBJ = build/pas_client.o

UTILS = src/shared/utils_v3.c
SERVER = src/server/pas_server.c
CLIENT = src/client/pas_client.c

# === Compilation ===
all: build pas_server

build:
	mkdir -p build

pas_server: pas_server.o utils_v3.o
	$(CC) $(CFLAGS) -o build/pas_server $(SERVER_OBJ) $(UTILS_OBJ)

pas_client: pas_client.o
	$(CC) $(CFLAGS) -o build/pas_client $(CLIENT_OBJ) $(UTILS_OBJ)

pas_server.o: $(SERVER) utils_v3.o
	$(CC) $(CFLAGS) -c $(SERVER) -o $(SERVER_OBJ)

pas_client.o: $(CLIENT) utils_v3.o
	$(CC) $(CFLAGS) -c $(CLIENT) -o $(CLIENT_OBJ)

utils_v3.o: $(UTILS)
	$(CC) $(CFLAGS) -c $(UTILS) -o $(UTILS_OBJ)

# === Clean ===
clean:
	rm -f build/*

