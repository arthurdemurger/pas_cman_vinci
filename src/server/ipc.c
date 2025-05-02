#include "ipc.h"

// Function to initialize shared memory and reset the game state
struct GameState* initialize_shared_memory(void) {
  int shm_id = sshmget(IPC_PRIVATE, sizeof(struct GameState), IPC_CREAT | PERM);
  struct GameState *shm_ptr = (struct GameState *) sshmat(shm_id);
  reset_gamestate(shm_ptr);
  return shm_ptr;
}

// Function to initialize a semaphore
int initialize_semaphore(void) {
  int sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | PERM);
  checkNeg(sem_id, "Semget error");

  union semun arg;
  arg.val = 0; // semaphore value 0 -> blocked
  if (semctl(sem_id, 0, SETVAL, arg) == -1) {
    perror("Error semctl in sem_create");
    exit(EXIT_FAILURE);
  }
  return sem_id;
}
