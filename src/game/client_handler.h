#ifndef _CLIENT_HANDLER_H_
#define _CLIENT_HANDLER_H_


/************************************
 * INCLUDES
 ************************************/
#include "../shared/utils_v3.h"
#include "../shared/pascman.h"
#include "game.h"
#include "../server/pas_server.h"

/************************************
 * STRUCTURES
 ************************************/
typedef struct {
  int sockfd;
  enum Item player;
  FileDescriptor broadcaster_pipe;
} ClientHandlerArgs;

/************************************
 * FUNCTIONS
 ************************************/
/**
 * PRE:  state: a pointer to the ServerState structure, properly initialized with client sockets and shared memory.
 * POST: Forks and runs two client handler processes to manage client interactions.
 *       Notifies players of their successful registration using the shared memory.
 * RES:  None.
 */
void launch_client_handler(ServerState* state);

#endif // _CLIENT_HANDLER_H_
