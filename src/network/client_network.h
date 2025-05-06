#ifndef _CLIENT_NETWORK_H_
#define _CLIENT_NETWORK_H_

/************************************
 * INCLUDES
 ************************************/
/* Libraries */
#include <sys/socket.h>
#include <stdlib.h>
/* Files */
#include "../shared/utils_v3.h"
#include "../client/pas_client_utils.h"

/************************************
 * FUNCTIONS
 ************************************/
/**
 * PRE:  client_state: a pointer to a ClientState structure.
 * POST: Creates a socket, connects to the server at server_ip:server_port, and verifies the server's response.
 *       If the server accepts the connection, the socket file descriptor is stored in client_state->sock_fd.
 *       If the server refuses the connection or an error occurs, the program exits with an error message.
 */
void connect_to_server(ClientState *client_state);

#endif // _CLIENT_NETWORK_H_
