#ifndef _IPC_H_

#define _IPC_H_

#include <sys/shm.h>
#include <sys/sem.h>
#include "../shared/utils_v3.h"
#include "../game/game.h"

#define PERM 0666

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

#endif // _IPC_H_
