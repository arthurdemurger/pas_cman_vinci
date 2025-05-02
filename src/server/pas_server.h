#ifndef __PAS_SERVER_H__
#define __PAS_SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "../ui/student_kit/pascman.h"
#include "./game.h"
#include "../shared/utils_v3.h"
#include <sys/sem.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct {
  int client_sockets[2];
  int clients_connected;
} ServerState;

/**
 * PRE:  serverPort: a valid port number
 * POST: on success, binds a socket to 0.0.0.0:serverPort and listens to it ;
 *       on failure, displays error cause and quits the program
 * RES: return socket file descriptor
 */
int initSocketServer(int serverPort);

/**
 * PRE:  None
 * POST: Creates a semaphore set with one semaphore and initializes it to 0 (blocked).
 *       On failure, displays an error message and exits the program.
 * RES:  Returns the semaphore ID.
 */
int initialize_semaphore(void);

/**
 * PRE:  None
 * POST: Creates a shared memory segment for the GameState structure and resets its state.
 *       On failure, displays an error message and exits the program.
 * RES:  Returns a pointer to the shared memory segment.
 */
struct GameState* initialize_shared_memory(void);

#endif // __PAS_SERVER_H__
