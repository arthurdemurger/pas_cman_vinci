#include "./pas_server.h"

int main (void) {
  // Declare a GameState structure to hold the game state
  struct GameState state;

  // Create a shared memory segment for the GameState structure
  int shm_id = sshmget(IPC_PRIVATE, sizeof(struct GameState), IPC_CREAT | 0666);

  // Attach the shared memory segment to the process's address space
  struct GameState *shm_ptr = (struct GameState *) sshmat(shm_id);

  // Reset the game state in the shared memory
  reset_gamestate(shm_ptr);

  // Create a semaphore set with one semaphore
  int sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
  checkNeg(sem_id, "Semget error");

  // Initialize the semaphore value
  union semun arg;
  arg.val = 0; // semaphore value 0 -> blocked

  if (semctl(sem_id, 0, SETVAL, arg) == -1) { // Apply the value to the semaphore
    perror("Error semctl in sem_create");
    exit(EXIT_FAILURE);
  }

  int pipe_to_broadcaster[2];
  spipe(pipe_to_broadcaster);

}
