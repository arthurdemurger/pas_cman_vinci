#ifndef _CLIENT_HANDLER_H_
#define _CLIENT_HANDLER_H_

#include "../shared/utils_v3.h"
#include "../game/game.h"
#include "pas_server.h"

typedef struct {
  int sockfd;
  enum Item player;
  FileDescriptor broadcaster_pipe;
} ClientHandlerArgs;

/**
 * PRE:  arg: a pointer to a ClientHandlerArgs structure containing the socket file descriptor,
 *            player identifier, and broadcaster pipe.
 * POST: Handles client commands in a loop, processes user commands, and communicates with the game state.
 *       Exits when the client disconnects or the game finishes.
 * RES:  None. The function exits the process on completion.
 */
void run_client_handler(void* arg);

/**
 * PRE:  state: a pointer to the ServerState structure, properly initialized with client sockets and shared memory.
 * POST: Forks and runs two client handler processes to manage client interactions.
 *       Notifies players of their successful registration using the shared memory.
 * RES:  None.
 */
void launch_client_handler(ServerState* state);

#endif // _CLIENT_HANDLER_H_
