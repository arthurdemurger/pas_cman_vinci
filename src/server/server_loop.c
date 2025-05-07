#include "server_loop.h"

static volatile sig_atomic_t interrupted = 0;

/**
 * Resets the server state after a game ends.
 *
 * PRE:
 *   - `state` is a valid pointer to an initialized ServerState structure.
 * POST:
 *   - All resources (pipes, sockets, processes) related to the game are cleaned up.
 *   - The game state and server state are reset to their initial values.
 */
static void reset_after_game(ServerState* state);

void sigint_handler(int sig) {
  (void)sig;
  interrupted = 1;
}

static void reset_after_game(ServerState* state) {

  // wait for the client handlers and broadcaster to finish
  waitpid(state->client_handler_pids[0], NULL, 0);
  waitpid(state->client_handler_pids[1], NULL, 0);
  waitpid(state->broadcaster_pid, NULL, 0);

  // Close the client sockets and broadcaster pipe
  for (int i = 0; i < state->clients_connected; ++i) {
    close(state->client_sockets[i]);
    state->client_sockets[i] = 0;
  }
  close(state->broadcaster_pipe);

  // reset the game state
  reset_gamestate(state->shm_ptr);

  // reset the server state
  state->clients_connected = 0;
  state->client_handler_pids[0] = 0;
  state->client_handler_pids[1] = 0;
  state->broadcaster_pid = 0;
  state->broadcaster_pipe = -1;
}

void run_server(ServerState* state, const char* map_path) {
  while (!interrupted) {
    print_server_msg("Starting a new game...");
    accept_clients(state);
    launch_broadcaster(state, map_path);
    launch_client_handler(state);

    print_server_msg("Waiting for game to finish...");
    while (!state->shm_ptr->game_over) {
      usleep(100000);
    }
    print_server_msg("Game over");

    reset_after_game(state);
    print_server_msg("Server state reset");
  }
}
