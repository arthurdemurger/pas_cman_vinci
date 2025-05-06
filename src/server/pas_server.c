#include "pas_server.h"

static ServerState* server_state_ptr = NULL;

void print_server_msg(const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf("\033[1;32m[SERVER]\033[0m ");
  vprintf(format, args);
  printf("\n");
  va_end(args);
}

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

  // launch the server loop with the map path
  run_server(&state, av[2]);

  // Clean up resources
  cleanup_resources(&state);
  return 0;
}
