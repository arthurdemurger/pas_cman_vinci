#include "pas_server.h"

static ServerState* server_state_ptr = NULL;

ServerState* get_server_state(void) {
  return server_state_ptr;
}

int main(void) {
  // Initialize shared memory and reset the game state
  struct GameState *shm_ptr = initialize_shared_memory();
  printf("Shared memory initialized.\n");

  // Initialize semaphore
  int sem_id = initialize_semaphore();
  printf("Semaphore initialized.\n");

  // Initialize socket server
  int server_socket = init_socket_server(SERVER_PORT);
  printf("Server started on %s:%d\n", SERVER_IP, SERVER_PORT);

  ServerState state = {0};
  server_state_ptr = &state;
  signal(SIGALRM, timeout_handler);

  // Accept clients
  accept_clients(server_socket, &state);
  printf("Clients accepted.\n");

  int pipe_fds[2];
  spipe(pipe_fds); // pipe entre le serveur et le broadcaster

  FileDescriptor fd_write_broad = pipe_fds[1];
  FileDescriptor fd_read_broad = pipe_fds[0];

  // Create the broadcaster process
  pid_t broad_pid = fork_and_run1(run_broadcaster, (void *)(intptr_t)fd_read_broad);
  printf("Broadcaster process created with PID\n");
}
