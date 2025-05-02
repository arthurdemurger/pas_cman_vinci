# === Compilation flags ===
CC = gcc
CFLAGS = -std=c17 -pedantic -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable -D_DEFAULT_SOURCE

# === Fichiers sources ===
SERVER_SRC = src/server/pas_server.c
CLIENT_SRC = src/client/pas_client.c
UTILS_SRC  = src/shared/utils_v3.c
GAME_SRC   = src/server/game.c

# === Fichiers headers ===
SERVER_H = src/server/pas_server.h
CLIENT_H = src/client/pas_client.h
UTILS_H  = src/shared/utils_v3.h
GAME_H   = src/server/game.h

# === Fichiers objets ===
SERVER_OBJ = build/pas_server.o
CLIENT_OBJ = build/pas_client.o
UTILS_OBJ  = build/utils_v3.o
GAME_OBJ   = build/game.o

# === Programmes compilés ===
SERVER_BIN = build/pas_server
CLIENT_BIN = build/pas_client

# === Règles principales ===
all: build $(SERVER_BIN) $(CLIENT_BIN)

build:
	mkdir -p build

$(SERVER_BIN): $(SERVER_OBJ) $(UTILS_OBJ) $(GAME_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT_BIN): $(CLIENT_OBJ) $(UTILS_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# === Règles de compilation des objets ===
$(SERVER_OBJ): $(SERVER_SRC) $(SERVER_H) $(UTILS_H) $(GAME_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_OBJ): $(CLIENT_SRC) $(CLIENT_H) $(UTILS_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(UTILS_OBJ): $(UTILS_SRC) $(UTILS_H)
	$(CC) $(CFLAGS) -c $< -o $@

$(GAME_OBJ): $(GAME_SRC) $(GAME_H)
	$(CC) $(CFLAGS) -c $< -o $@

# === Nettoyage ===
clean:
	rm -f build/*.o build/pas_server build/pas_client

.PHONY: all clean build
