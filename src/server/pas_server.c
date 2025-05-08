#include "pas_server.h"

static ServerState* server_state_ptr = NULL;

ServerState* get_server_state(void) {
  return server_state_ptr;
}

int main(int ac, char **av) {
  if (ac != 3) {
    fprintf(stderr, "Usage: %s <port> <path/map.txt>\n", av[0]);
    exit(EXIT_FAILURE);
  }

  // Set up signal handler for SIGINT
  ssigaction(SIGINT, sigint_handler);

  // ServerState state = init_server_state(server_state_ptr);
  ServerState state = {0};
  server_state_ptr = &state;
  state.server_port = atoi(av[1]);
  check_port(state.server_port);

  // Initialize shared memory and reset the game state
  initialize_shared_memory(&state);

  // Initialize semaphore
  initialize_semaphore(&state);

  // Initialize socket server
  init_socket_server(&state);

  char *map_path = av[2];
  // launch the server loop with the map path
  run_server(&state, map_path);

  // Clean up resources
  cleanup_resources(&state);
  return 0;
}
