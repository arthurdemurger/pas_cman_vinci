#ifndef _CLIENT_HANDLER_H_
#define _CLIENT_HANDLER_H_

#include "../shared/utils_v3.h"
#include "../game/game.h"

/**
 * PRE:  state: a pointer to the ServerState structure, properly initialized with client sockets and shared memory.
 * POST: Forks and runs two client handler processes to manage client interactions.
 *       Notifies players of their successful registration using the shared memory.
 */
void launch_client_handler(ServerState* state);

#endif // _CLIENT_HANDLER_H_
