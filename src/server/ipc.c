#include "ipc.h"

// Function to initialize shared memory and reset the game state
void initialize_shared_memory(ServerState* state) {
  int shm_id = sshmget(IPC_KEY, sizeof(struct GameState), IPC_CREAT | PERM);
  struct GameState *shm_ptr = (struct GameState *) sshmat(shm_id);
  reset_gamestate(shm_ptr);
  state->shm_id = shm_id;
  state->shm_ptr = shm_ptr;
  printf("Shared memory initialized.\n");
}

// Function to initialize a semaphore
void initialize_semaphore(ServerState* state) {
  int sem_id = semget(IPC_KEY, 1, IPC_CREAT | PERM);
  checkNeg(sem_id, "Semget error");

  union semun arg;
  arg.val = 0; // semaphore value 0 -> blocked
  if (semctl(sem_id, 0, SETVAL, arg) == -1) {
    perror("Error semctl in sem_create");
    exit(EXIT_FAILURE);
  }

  state->sem_id = sem_id;
  printf("Semaphore initialized.\n");
}

void cleanup_resources(ServerState* state) {
  if (state->shm_ptr != NULL) {
    sshmdt(state->shm_ptr);
  }

  if (state->shm_id > 0) {
    sshmdelete(state->shm_id);
  }

  if (state->sem_id > 0) {
    sem_delete(state->sem_id);
  }

  printf("Ressources IPC nettoyées correctement.\n");
}
