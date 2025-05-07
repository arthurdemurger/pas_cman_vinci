# === Compilation flags ===
CC = gcc
CFLAGS = -std=c17 -pedantic -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable -D_DEFAULT_SOURCE

# === Color codes ===
COLOR_SERVER=\033[34m  # Blue
COLOR_CLIENT=\033[32m  # Green
COLOR_PAS_LABO=\033[33m  # Yellow
COLOR_RESET=\033[0m  # Reset to default color

# === Sources files ===
# ======================== SERVER SOURCES ========================
SERVER_SRC         = src/server/pas_server.c
SERVER_UTILS_SRC    = src/server/server_utils.c
IPC_SRC            = src/server/ipc.c
BROADCASTER_SRC    = src/server/broadcaster.c
CLIENT_HANDLER_SRC = src/game/client_handler.c
SERVER_NETWORK_SRC = src/network/server_network.c
# ======================== CLIENT SOURCES ========================
CLIENT_SRC           = src/client/pas_client.c
CLIENT_NETWORK_SRC   = src/network/client_network.c
CLIENT_UTILS_SRC     = src/client/pas_client_utils.c
UI_HANDLER_SRC       = src/client/ui_handler.c
# ======================== SHARED SOURCES ========================
UTILS_SRC          = src/shared/utils_v3.c
# ======================== GAME SOURCES ========================
GAME_SRC           = src/game/game.c
# ======================== PAS LABO SOURCES ========================
PAS_LABO_SRC       = src/labo/pas_labo.c

# === Header files ===
# ======================== SERVER HEADERS ========================
SERVER_H         = src/server/pas_server.h
SERVER_UTILS_H   = src/server/server_utils.h
IPC_H            = src/server/ipc.h
BROADCASTER_H    = src/server/broadcaster.h
CLIENT_HANDLER_H = src/game/client_handler.h
SERVER_NETWORK_H = src/network/server_network.h
# ======================== CLIENT HEADERS ========================
CLIENT_H         = src/client/pas_client.h
CLIENT_NETWORK_H = src/network/client_network.h
CLIENT_UTILS_H   = src/client/pas_client_utils.h
UI_HANDLER_H     = src/client/ui_handler.h
# ======================== SHARED HEADERS ========================
UTILS_H          = src/shared/utils_v3.h
# ======================== GAME HEADERS ========================
GAME_H           = src/game/game.h
# ======================== PAS LABO HEADERS ========================
PAS_LABO_H       = src/labo/pas_labo.h

# === Object files ===
# ======================== SERVER OBJECTS ========================
SERVER_OBJ         = build/pas_server.o
SERVER_UTILS_OBJ   = build/server_utils.o
IPC_OBJ            = build/ipc.o
BROADCASTER_OBJ    = build/broadcaster.o
CLIENT_HANDLER_OBJ = build/client_handler.o
SERVER_NETWORK_OBJ = build/server_network.o
# ======================== CLIENT OBJECTS ========================
CLIENT_OBJ           = build/pas_client.o
CLIENT_NETWORK_OBJ   = build/client_network.o
CLIENT_UTILS_OBJ     = build/client_utils.o
UI_HANDLER_OBJ       = build/ui_handler.o
# ======================== SHARED OBJECTS ========================
UTILS_OBJ          = build/utils_v3.o
# ======================== GAME OBJECTS ========================
GAME_OBJ           = build/game.o
# ======================== PAS LABO OBJECTS ========================
PAS_LABO_OBJ       = build/pas_labo.o

# === Compiled programs ===
PAS_SERVER_BIN = build/pas_server
PAS_CLIENT_BIN = build/pas_client
PAS_LABO_BIN = build/pas_labo

# === Principal rules ===
all: build $(PAS_SERVER_BIN) $(PAS_CLIENT_BIN) $(PAS_LABO_BIN)

build:
	@mkdir -p build
	@echo "Build directory created."

# === Binary rules ===
# ======================== SERVER BINARY ========================
$(PAS_SERVER_BIN): $(SERVER_OBJ) $(SERVER_NETWORK_OBJ) $(SERVER_UTILS_OBJ) $(IPC_OBJ) $(BROADCASTER_OBJ) $(CLIENT_HANDLER_OBJ) $(UTILS_OBJ) $(GAME_OBJ)
	@echo "${COLOR_SERVER}Linking $@...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "${COLOR_SERVER}Linked $@.${COLOR_RESET}"

# ======================== CLIENT BINARY ========================
$(PAS_CLIENT_BIN): $(CLIENT_OBJ) $(CLIENT_NETWORK_OBJ) $(CLIENT_UTILS_OBJ) $(UI_HANDLER_OBJ) $(UTILS_OBJ)
	@echo "${COLOR_CLIENT}Linking $@...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "${COLOR_CLIENT}Linked $@.${COLOR_RESET}"

# ======================== PAS LABO BINARY ========================
$(PAS_LABO_BIN): $(PAS_LABO_OBJ) $(UTILS_OBJ)
	@echo "${COLOR_PAS_LABO}Linking $@...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "${COLOR_PAS_LABO}Linked $@.${COLOR_RESET}"

# === Objects rules ===
# ======================== SERVER OBJECTS ========================
$(SERVER_OBJ): $(SERVER_SRC) $(SERVER_H) $(UTILS_H) $(GAME_H)
	@echo "${COLOR_SERVER}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_UTILS_OBJ): $(SERVER_UTILS_SRC) $(SERVER_UTILS_H) $(IPC_H) $(SERVER_H)
	@echo "${COLOR_SERVER}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

$(IPC_OBJ): $(IPC_SRC) $(IPC_H) $(SERVER_H)
	@echo "${COLOR_SERVER}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

$(BROADCASTER_OBJ): $(BROADCASTER_SRC) $(BROADCASTER_H) $(IPC_H)
	@echo "${COLOR_SERVER}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_HANDLER_OBJ): $(CLIENT_HANDLER_SRC) $(CLIENT_HANDLER_H) $(IPC_H) $(UTILS_H)
	@echo "${COLOR_SERVER}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_NETWORK_OBJ): $(SERVER_NETWORK_SRC) $(SERVER_NETWORK_H)
	@echo "${COLOR_SERVER}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

# ======================== CLIENT OBJECTS ========================
$(CLIENT_OBJ): $(CLIENT_SRC) $(CLIENT_H) $(UTILS_H) $(CLIENT_UTILS_H) $(CLIENT_NETWORK_H) $(UI_HANDLER_H)
	@echo "${COLOR_CLIENT}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_NETWORK_OBJ): $(CLIENT_NETWORK_SRC) $(CLIENT_NETWORK_H)
	@echo "${COLOR_CLIENT}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_UTILS_OBJ): $(CLIENT_UTILS_SRC) $(CLIENT_UTILS_H) $(UTILS_H)
	@echo "${COLOR_CLIENT}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

$(UI_HANDLER_OBJ): $(UI_HANDLER_SRC) $(UI_HANDLER_H) $(CLIENT_H) $(CLIENT_NETWORK_H)
	@echo "${COLOR_CLIENT}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

# ======================== SHARED OBJECTS ========================
$(UTILS_OBJ): $(UTILS_SRC) $(UTILS_H)
	@echo "${COLOR_SERVER}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

# ======================== GAME OBJECTS ========================
$(GAME_OBJ): $(GAME_SRC) $(GAME_H)
	@echo "${COLOR_SERVER}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

# ======================== PAS LABO OBJECTS ========================
$(PAS_LABO_OBJ): $(PAS_LABO_SRC) $(PAS_LABO_H) $(UTILS_H)
	@echo "${COLOR_PAS_LABO}Compiling $<...${COLOR_RESET}"
	@$(CC) $(CFLAGS) -c $< -o $@

# === Cleaning ===
clean:
	@echo "Cleaning build artifacts..."
	@rm -f build/*.o build/pas_server build/pas_client build/pas_labo
	@echo "Cleaned."

.PHONY: all clean build
