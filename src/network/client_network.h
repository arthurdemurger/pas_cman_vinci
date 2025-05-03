#ifndef _CLIENT_NETWORK_H_
#define _CLIENT_NETWORK_H_

#include <sys/socket.h>
#include "../shared/utils_v3.h"

/**
 * PRE: serverPort: a valid port number
 * POST: on success, connects a client socket to SERVERIP:serverPort
 *       on failure, displays error cause and quits the program
 * RES: return socket file descriptor
 */
int init_socket_client(int serverPort);

#endif // _CLIENT_NETWORK_H_
