#ifndef _SERVER_UTILS_H_
#define _SERVER_UTILS_H_

/************************************
 * INCLUDES
 ************************************/
/* Files */
#include "../shared/utils_v3.h"
#include "../game/game.h"
#include "../network/server_network.h"
#include "broadcaster.h"

/************************************
 * FUNCTIONS
 ************************************/
/**
 * Runs the server loop, accepting clients, launching game components, and resetting after each game.
 *
 * PRE:
 *   - `state` is a valid pointer to an initialized ServerState structure.
 *   - `map_path` is a valid path to the map file.
 * POST:
 *   - The server runs until interrupted, handling multiple game sessions.
 *   - After each game, the server state is reset.
 */
void run_server(ServerState* state, const char* map_path);

/**
 * Handles the SIGINT signal by setting the `interrupted` flag.
 *
 * PRE:
 *   - None.
 * POST:
 *   - The `interrupted` flag is set to 1 when SIGINT is received.
 */
void sigint_handler(int sig);

/**
 * Initializes the server state structure.
 *
 * PRE:
 *   - `server_state_ptr` is a valid pointer to a ServerState structure.
 * POST:
 *   - The ServerState structure is initialized with default values.
 *   - Returns the initialized ServerState structure.
 * RES:
 *   - Returns a ServerState structure with all fields set to default values.
 */
ServerState init_server_state(ServerState* server_state_ptr);

/**
 * Cleans up resources used by the server, including sockets and IPC.
 *
 * PRE:
 *   - `state` is a valid pointer to an initialized ServerState structure.
 * POST:
 *   - All resources associated with the server are released.
 */
void cleanup_resources(ServerState* state);

/**
 * Reads data from a file descriptor safely, handling interruptions.
 *
 * PRE:
 *   - `fd` is a valid file descriptor.
 *   - `buf` is a pointer to the buffer where data will be read.
 *   - `count` is the number of bytes to read.
 * POST:
 *   - Reads data from the file descriptor into the buffer.
 * RES:
 *   - Returns the number of bytes read, or -1 on error.
 */
ssize_t safe_read(int fd, void *buf, size_t count);

/**
 * Receives data from a socket safely, handling interruptions.
 *
 * PRE:
 *   - `sockfd` is a valid socket file descriptor.
 *   - `buf` is a pointer to the buffer where data will be received.
 *   - `len` is the number of bytes to receive.
 *   - `flags` are the flags for the recv function.
 * POST:
 *   - Receives data from the socket into the buffer.
 * RES:
 *   - Returns the number of bytes received, or -1 on error.
 */
ssize_t safe_recv(int sockfd, void *buf, size_t len, int flags);

#endif // _SERVER_UTILS_H_
