#ifndef _SERVER_NETWORK_H_
#define _SERVER_NETWORK_H_

#include <sys/socket.h>
#include <unistd.h>
#include "../shared/utils_v3.h"
#include "../server/pas_server.h"
#include <stdlib.h>

#define MAX_PLAYERS 2
#define BACKLOG 2

/**
 * PRE:  state: a pointer to a ServerState structure with the server port properly set.
 * POST: Creates a socket, binds it to 0.0.0.0:server_port, and starts listening for connections.
 *       Updates the ServerState structure with the server socket file descriptor.
 *       On failure, displays an error message and exits the program.
 */
void init_socket_server(ServerState* state);

/**
 * PRE:  sig: the signal number (expected to be SIGALRM).
 * POST: If one client is connected, closes the client's socket and resets the server state.
 *       Displays a message indicating the client was disconnected due to timeout.
 */
void timeout_handler(int sig);

/**
 * PRE:  state: a pointer to a ServerState structure with a valid server socket.
 * POST: Accepts up to MAX_PLAYERS client connections.
 *       Updates the ServerState structure with connected client information.
 *       Handles timeout for the second client if only one client is connected.
 *       Displays messages for each connected client.
 */
void accept_clients(ServerState *state);

#endif // _SERVER_NETWORK_H_
