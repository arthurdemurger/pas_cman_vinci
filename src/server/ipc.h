#ifndef _IPC_H_

#define _IPC_H_

#include <sys/shm.h>
#include <sys/sem.h>
#include "../shared/utils_v3.h"
#include "../game/game.h"

#define PERM 0666
#define IPC_KEY 1234
/**
 * PRE:  None
 * POST: Creates a semaphore set with one semaphore and initializes it to 0 (blocked).
 *       On failure, displays an error message and exits the program.
 * RES:  Returns the semaphore ID.
 */
int initialize_semaphore(void);

/**
 * PRE:  out_shm_id: a pointer to an integer where the shared memory ID will be stored.
 * POST: Creates a shared memory segment for the GameState structure and resets its state.
 *       Stores the shared memory ID in the provided pointer.
 *       On failure, displays an error message and exits the program.
 * RES:  Returns a pointer to the shared memory segment.
 */
struct GameState* initialize_shared_memory(int* out_shm_id);

#endif // _IPC_H_
