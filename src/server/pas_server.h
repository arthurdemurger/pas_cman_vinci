#ifndef _PAS_SERVER_H_
#define _PAS_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "../ui/student_kit/pascman.h"
#include "../game/game.h"
#include "../network/server_network.h"
#include "../shared/utils_v3.h"
#include <sys/sem.h>

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

/**
 * PRE:  None
 * POST: Returns a pointer to the current ServerState structure.
 *       If the ServerState is not initialized, returns NULL.
 * RES:  Pointer to the ServerState structure or NULL if uninitialized.
 */
ServerState* get_server_state(void);

#endif // _PAS_SERVER_H_
