#ifndef _BROADCASTER_H_
#define _BROADCASTER_H_

#include "../network/server_network.h"
#include "../shared/utils_v3.h"
#include "../server/pas_server.h"

/**
 * PRE:  arg: a pointer to a structure containing the necessary data for the broadcaster,
 *            including the pipe read file descriptor and any other required context.
 * POST: Continuously reads messages from the pipe and broadcasts them to all connected clients.
 *       Uses the ServerState structure to access client sockets.
 *       On failure, displays an error message and exits the program.
 */
void run_broadcaster(void *arg);

#endif // _BROADCASTER_H_
