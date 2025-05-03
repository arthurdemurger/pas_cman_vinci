#ifndef _CLIENT_NETWORK_H_
#define _CLIENT_NETWORK_H_

#include <sys/socket.h>
#include <stdlib.h>

#include "../shared/utils_v3.h"

/**
 * PRE:  server_port: a valid port number (> 0).
 * POST: Creates a socket, connects to the server at localhost:server_port, and verifies the server's response.
 *       If the server accepts the connection, the function returns the connected socket file descriptor.
 *       If the server refuses the connection or an error occurs, the program exits with an error message.
 * RES:  Returns the connected socket file descriptor on success.
 */
int connect_to_server(int server_port);

#endif // _CLIENT_NETWORK_H_
