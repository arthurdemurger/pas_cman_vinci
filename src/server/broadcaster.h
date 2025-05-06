#ifndef _BROADCASTER_H_
#define _BROADCASTER_H_

/************************************
 * INCLUDES
 ************************************/
/* Files */
#include "../network/server_network.h"
#include "../shared/utils_v3.h"
#include "pas_server.h"

/************************************
 * FUNCTIONS
 ************************************/
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
