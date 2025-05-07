#include "server_utils.h"

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

ServerState init_server_state(ServerState* server_state_ptr) {
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

  return state;
}

void cleanup_resources(ServerState* state) {
  // Close the server socket
  if (state->server_socket != -1) {
    close(state->server_socket);
  }
  // Close the client sockets
  for (int i = 0; i < state->clients_connected; ++i) {
    if (state->client_sockets[i] != -1) {
      close(state->client_sockets[i]);
    }
  }
  // Close the broadcaster pipe
  if (state->broadcaster_pipe != -1) {
    close(state->broadcaster_pipe);
  }

  // Clean up IPC resources
  cleanup_ipc(state);

  // Clean up the server state
  state->server_socket = -1;
  state->server_port = -1;
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
  state->client_handler_pids[0] = -1;
  state->client_handler_pids[1] = -1;
  state->broadcaster_pid = -1;
  state->broadcaster_pipe = -1;
}

ssize_t safe_recv(int sockfd, void *buf, size_t len, int flags) {
  ssize_t ret;
  while (1) {
    ret = recv(sockfd, buf, len, flags);
    if (ret < 0) {
      if (errno == EINTR) {
        print_server_msg("Signal received, still receiving...");
        continue;
      }
      else {
        perror("Recv error");
        cleanup_resources(get_server_state());
        exit(EXIT_FAILURE);
      }
    }
    break;
  }
  print_server_msg("Received %zd bytes", ret);
  return ret;
}

ssize_t safe_read(int fd, void *buf, size_t count) {
  ssize_t ret;
  while (1) {
    ret = read(fd, buf, count);
    if (ret < 0) {
      if (errno == EINTR) {
        print_server_msg("Signal received, still receiving...");
        continue;
      }
      else {
        perror("Read error");
        cleanup_resources(get_server_state());
        exit(EXIT_FAILURE);
      }
    }
    break;
  }
  return ret;
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
