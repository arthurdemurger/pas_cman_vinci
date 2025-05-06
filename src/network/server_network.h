#ifndef _SERVER_NETWORK_H_
#define _SERVER_NETWORK_H_

/************************************
 * CONSTANTS
 ************************************/
#define BACKLOG 2

/************************************
 * INCLUDES
 ************************************/
/* Libraries */
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
/* Files */
#include "../shared/utils_v3.h"
#include "../server/pas_server.h"

/************************************
 * FUNCTIONS
 ************************************/
/**
 * PRE:  state: a pointer to a ServerState structure with the server port properly set.
 * POST: Creates a socket, binds it to 0.0.0.0:server_port, and starts listening for connections.
 *       Updates the ServerState structure with the server socket file descriptor.
 *       On failure, displays an error message and exits the program.
 */
void init_socket_server(ServerState* state);


/**
 * PRE:  state: a pointer to a ServerState structure with a valid server socket.
 * POST: Accepts up to MAX_PLAYERS client connections.
 *       Updates the ServerState structure with connected client information.
 *       Handles timeout for the second client if only one client is connected.
 *       Displays messages for each connected client.
 */
void accept_clients(ServerState *state);

#endif // _SERVER_NETWORK_H_
