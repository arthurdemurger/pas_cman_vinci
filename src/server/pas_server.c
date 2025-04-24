#include "./pas_server.h"

int main (void) {
  struct GameState state;

  int shm_id = sshmget(IPC_PRIVATE, sizeof(struct GameState), IPC_CREAT | 0666);

  struct GameState *shm_ptr = (struct GameState *)sshmat(shm_id, NULL, 0);

  int sem_id = ssemget(IPC_PRIVATE, 1, IPC_CREAT | 0666);


  union semun arg;
  arg.val = 1;
  ssemctl(sem_id, 0, SETVAL, arg);
}
