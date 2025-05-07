#include "pas_server.h"

static ServerState* server_state_ptr = NULL;

void print_server_msg(const char *format, ...) {
  va_list args;
  va_start(args, format);
  colorOn(1, GREEN_TEXT);
  printf("[SERVER] ");
  colorOff();
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

  // ServerState state = init_server_state(server_state_ptr);
  ServerState state = {0};
  server_state_ptr = &state;
  state.broadcaster_pipe = -1;
  state.server_socket = -1;
  state.server_port = -1;
  state.clients_connected = 0;
  state.broadcaster_pid = -1;
  state.client_handler_pids[0] = -1;
  state.client_handler_pids[1] = -1;
  state.client_sockets[0] = -1;
  state.client_sockets[1] = -1;
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
