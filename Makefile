# === Alias ===
CC = gcc
CFLAGS = -std=c17 -pedantic -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable -D_DEFAULT_SOURCE

# === Fichiers ===
UTILS_OBJ = src/ui/student_kit/utils_v3.o
SERVER = src/server/pas_server.c
CLIENT = src/client/pas_client.c

# === Compilation ===
all: pas_server pas_client

pas_server: pas_server.o $(UTILS_OBJ)
	$(CC) $(CFLAGS) -o build/pas_server build/pas_server.o $(UTILS_OBJ)

pas_client: pas_client.o $(UTILS_OBJ)
	$(CC) $(CFLAGS) -o build/pas_client build/pas_client.o $(UTILS_OBJ)

pas_server.o: $(SERVER)
	$(CC) $(CFLAGS) -c $(SERVER) -o build/pas_server.o

pas_client.o: $(CLIENT)
	$(CC) $(CFLAGS) -c $(CLIENT) -o build/pas_client.o


# === Clean ===
clean:
	rm -f build/*


