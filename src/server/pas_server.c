#include "./pas_server.h"

#define PERM 0666

// Function to initialize shared memory and reset the game state
struct GameState* initialize_shared_memory() {
  int shm_id = sshmget(IPC_PRIVATE, sizeof(struct GameState), IPC_CREAT | PERM);
  struct GameState *shm_ptr = (struct GameState *) sshmat(shm_id);
  reset_gamestate(shm_ptr);
  return shm_ptr;
}

// Function to initialize a semaphore
int initialize_semaphore() {
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


int main(void) {
  // Initialize shared memory and reset the game state}
  struct GameState *shm_ptr = initialize_shared_memory();

  // Initialize semaphore
  int sem_id = initialize_semaphore();

  // Initialize pipe
  int pipe_to_broadcaster[2];
  spipe(pipe_to_broadcaster);

}
