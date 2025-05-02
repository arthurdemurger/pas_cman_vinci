# === Alias ===
CC = gcc
CFLAGS = -std=c17 -pedantic -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable -D_DEFAULT_SOURCE

# === .h Files ===
UTILS_H = src/shared/utils_v3.h
SERVER_H = src/server/pas_server.h
CLIENT_H = src/client/pas_client.h
GAME_H = src/server/game.h

# === Sources ===
UTILS = src/shared/utils_v3.c
SERVER = src/server/pas_server.c
CLIENT = src/client/pas_client.c
GAME = src/server/game.c

# === Object Files ===
UTILS_OBJ = build/utils_v3.o
SERVER_OBJ = build/pas_server.o
CLIENT_OBJ = build/pas_client.o
GAME_OBJ = build/game.o

# === Compilation ===
all: build pas_server pas_client

build:
	mkdir -p build

pas_server: $(SERVER_OBJ) $(UTILS_OBJ) $(GAME_OBJ)
	$(CC) $(CFLAGS) -o build/pas_server $(SERVER_OBJ) $(UTILS_OBJ) $(GAME_OBJ)

pas_client: $(CLIENT_OBJ) $(UTILS_OBJ)
	$(CC) $(CFLAGS) -o build/pas_client $(CLIENT_OBJ) $(UTILS_OBJ)

$(SERVER_OBJ): $(SERVER) $(SERVER_H) $(UTILS_H) $(GAME_H)
	$(CC) $(CFLAGS) -c $(SERVER) -o $(SERVER_OBJ)

$(CLIENT_OBJ): $(CLIENT) $(CLIENT_H) $(UTILS_H)
	$(CC) $(CFLAGS) -c $(CLIENT) -o $(CLIENT_OBJ)

$(GAME_OBJ): $(GAME) $(GAME_H)
	$(CC) $(CFLAGS) -c $(GAME) -o $(GAME_OBJ)

$(UTILS_OBJ): $(UTILS) $(UTILS_H)
	$(CC) $(CFLAGS) -c $(UTILS) -o $(UTILS_OBJ)

# === Clean ===
clean:
	rm -f build/*
