#include "pas_server.h"

static ServerState* server_state_ptr = NULL;

ServerState* get_server_state(void) {
  return server_state_ptr;
}

int main(int ac, char **av) {
  if (ac != 2) {
    fprintf(stderr, "Usage: %s <port>\n", av[0]);
    exit(EXIT_FAILURE);
  }

  ServerState state = {0};
  server_state_ptr = &state;
  state.server_port = atoi(av[1]);

  // Initialize shared memory and reset the game state
  initialize_shared_memory(&state);

  // Initialize semaphore
  initialize_semaphore(&state);

  // Initialize socket server
  init_socket_server(&state);

  // Accept clients
  accept_clients(&state);

  // Launch the broadcaster
  pid_t broad_pid = launch_broadcaster(&state, "resources/map.txt");
  (void)broad_pid; // Suppress unused variable warning

  sleep(30);
  cleanup_resources(&state);
}
