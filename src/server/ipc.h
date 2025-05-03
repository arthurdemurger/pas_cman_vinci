#ifndef _IPC_H_

#define _IPC_H_

#include <sys/shm.h>
#include <sys/sem.h>
#include "../shared/utils_v3.h"
#include "../game/game.h"
#include "pas_server.h"

#define PERM 0666
#define IPC_KEY 1234
/**
 * PRE:  state: a pointer to the ServerState structure.
 * POST: Creates a semaphore set with one semaphore and initializes it to 0 (blocked).
 *       Updates the ServerState structure with the semaphore ID.
 *       On failure, displays an error message and exits the program.
 */
void initialize_semaphore(ServerState* state);

/**
 * PRE:  state: a pointer to the ServerState structure.
 * POST: Creates a shared memory segment for the GameState structure, attaches it, and resets its state.
 *       Updates the ServerState structure with the shared memory ID and pointer.
 *       On failure, displays an error message and exits the program.
 */
void initialize_shared_memory(ServerState* state);

/**
 * PRE:  state: a pointer to the ServerState structure.
 * POST: Cleans up the resources used by the server, including shared memory and semaphores.
 *       Displays a message indicating successful cleanup.
 */
void cleanup_resources(ServerState* state);

#endif // _IPC_H_
