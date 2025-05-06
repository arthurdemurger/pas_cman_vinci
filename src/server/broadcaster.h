#ifndef _BROADCASTER_H_
#define _BROADCASTER_H_

#include "../network/server_network.h"
#include "../shared/utils_v3.h"
#include "pas_server.h"

/**
 * PRE:  arg: a pointer to a structure containing the necessary data for the broadcaster,
 *            including the pipe read file descriptor and any other required context.
 * POST: Continuously reads messages from the pipe and broadcasts them to all connected clients.
 *       Uses the ServerState structure to access client sockets.
 *       On failure, displays an error message and exits the program.
 */
void run_broadcaster(void *arg);

/**
 * PRE:  state: a pointer to the ServerState structure.
 *       map_path: a valid path to the map file to be loaded.
 * POST: Creates a pipe for communication between the server and the broadcaster.
 *       Forks the broadcaster process and initializes it with the pipe's read end.
 *       Loads the map data into shared memory and sends it through the pipe.
 *       Updates the ServerState structure with the pipe's write end.
 */
void launch_broadcaster(ServerState* state, const char* map_path);

#endif // _BROADCASTER_H_
