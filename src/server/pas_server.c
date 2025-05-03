#include "pas_server.h"

static ServerState* server_state_ptr = NULL;

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


ServerState* get_server_state(void) {
  return server_state_ptr;
}

int main(void) {
  ServerState state = {0};
  server_state_ptr = &state;
  int shm_id;

  // Initialize shared memory and reset the game state
  struct GameState *shm_ptr = initialize_shared_memory(&shm_id);

  state.shm_id = shm_id;
  state.shm_ptr = shm_ptr;

  // Initialize semaphore
  int sem_id = initialize_semaphore();
  state.sem_id = sem_id;

  // Initialize socket server
  int server_socket = init_socket_server(SERVER_PORT);

  // Accept clients
  accept_clients(server_socket, &state);

  int pipe_fds[2];
  spipe(pipe_fds); // pipe entre le serveur et le broadcaster

  FileDescriptor fd_write_broad = pipe_fds[1];
  FileDescriptor fd_read_broad = pipe_fds[0];

  // Create the broadcaster process
  pid_t broad_pid = fork_and_run1(run_broadcaster, (void *)(intptr_t)fd_read_broad);

  sleep(30);

  cleanup_resources(&state);
}
