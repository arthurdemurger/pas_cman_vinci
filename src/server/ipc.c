#include "ipc.h"

// Function to initialize shared memory and reset the game state
struct GameState* initialize_shared_memory(int* out_shm_id) {
  int shm_id = sshmget(IPC_KEY, sizeof(struct GameState), IPC_CREAT | PERM);
  struct GameState *shm_ptr = (struct GameState *) sshmat(shm_id);
  reset_gamestate(shm_ptr);
  *out_shm_id = shm_id;

  printf("Shared memory initialized.\n");
  return shm_ptr;
}

// Function to initialize a semaphore
int initialize_semaphore(void) {
  int sem_id = semget(IPC_KEY, 1, IPC_CREAT | PERM);
  checkNeg(sem_id, "Semget error");

  union semun arg;
  arg.val = 0; // semaphore value 0 -> blocked
  if (semctl(sem_id, 0, SETVAL, arg) == -1) {
    perror("Error semctl in sem_create");
    exit(EXIT_FAILURE);
  }

  printf("Semaphore initialized.\n");
  return sem_id;
}
