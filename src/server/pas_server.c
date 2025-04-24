#include "./pas_server.h"

int main (void) {
  struct GameState state;

  int shm_id = sshmget(IPC_PRIVATE, sizeof(struct GameState), IPC_CREAT | 0666);

  struct GameState *shm_ptr = (struct GameState *) sshmat(shm_id);

  int sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
  checkNeg(sem_id, "Error semget in sem_create");

  union semun arg;
  arg.val = 1;
  if (semctl(sem_id, 0, SETVAL, arg) == -1) {
    perror("Error semctl in sem_create");
    exit(EXIT_FAILURE);
  }

  printf("Shared memory and semaphore created.\n");
}
