#include "server_loop.h"

static volatile sig_atomic_t interrupted = 0;

static void sigint_handler(int sig) {
  (void)sig;
  interrupted = 1;
}

void setup_sigint_handler(void) {
  struct sigaction sa;
  sa.sa_handler = sigint_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);
}

void reset_after_game(ServerState* state) {
  // Close the broadcaster pipe
  close(state->broadcaster_pipe);
  state->broadcaster_pipe = -1;

  // Close the client sockets
  for (int i = 0; i < state->clients_connected; ++i) {
    close(state->client_sockets[i]);
    state->client_sockets[i] = 0;
  }

  // wait for the client handlers and broadcaster to finish
  waitpid(state->client_handler_pids[0], NULL, 0);
  waitpid(state->client_handler_pids[1], NULL, 0);
  waitpid(state->broadcaster_pid, NULL, 0);

  // reset the game state
  reset_gamestate(state->shm_ptr);

  // reset the server state
  state->clients_connected = 0;
  state->client_handler_pids[0] = 0;
  state->client_handler_pids[1] = 0;
  state->broadcaster_pid = 0;

  print_server_msg("Game finished");
}

void run_server(ServerState* state, const char* map_path) {
  while (!interrupted) {
    accept_clients(state);
    launch_broadcaster(state, map_path);
    launch_client_handler(state);

    while (!state->shm_ptr->game_over) {
      usleep(100000);
    }

    reset_after_game(state);
  }
}
