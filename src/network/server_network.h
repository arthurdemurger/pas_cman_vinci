#ifndef _SERVER_NETWORK_H_

#define _SERVER_NETWORK_H_

#include <sys/socket.h>
#include <unistd.h>
#include "../shared/utils_v3.h"
#include "../server/pas_server.h"
#include <stdlib.h>

#define PERM 0666
#define MAX_PLAYERS 2
#define BACKLOG 2

/**
 * PRE:  serverPort: a valid port number
 * POST: on success, binds a socket to 0.0.0.0:serverPort and listens to it ;
 *       on failure, displays error cause and quits the program
 * RES: return socket file descriptor
 */
int init_socket_server(int serverPort);

/**
 * PRE:  sig: the signal number (expected to be SIGALRM).
 * POST: If one client is connected, closes the client's socket and resets the server state.
 *       Displays a message indicating the client was disconnected due to timeout.
 */
void timeout_handler(int sig);

/**
 * PRE:  server_socket: a valid server socket file descriptor.
 *       state: a pointer to a ServerState structure to manage connected clients.
 * POST: Accepts up to MAX_PLAYERS client connections.
 *       Spawns a child process for each client to handle its connection.
 *       Updates the ServerState structure with connected client information.
 */
void accept_clients(int server_socket, ServerState *state);

#endif // _SERVER_NETWORK_H_
