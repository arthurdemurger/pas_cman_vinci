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

// /**
//  * Safely waits for a process to finish, handling interruptions.
//  *
//  * PRE:
//  *   - `pid` is the PID of the process to wait for.
//  *   - `wstatus` is a pointer to an integer where the exit status will be stored.
//  *   - `options` are the options for the waitpid call.
//  * POST:
//  *   - Waits for the specified process to finish, handling interruptions.
//  * RES:
//  *   - Returns the PID of the finished process or -1 on error.
//  */
// static pid_t safe_waitpid(pid_t pid, int *wstatus, int options);

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

int get_interrupted(void) {
  return interrupted;
}

void sigint_handler(int sig) {
  (void)sig;
  interrupted = 1;
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

static pid_t safe_waitpid(pid_t pid, int *wstatus, int options) {
  pid_t ret;
  print_server_msg("Waiting for process %d to finish...", pid);
  while (1) {
    ret = swaitpid(pid, wstatus, options);
    if (ret < 0) {
      if (errno == EINTR && interrupted) {
        continue;
      }
      else {
        // print_server_msg("FUCK");
        // perror("Waitpid error");
        cleanup_resources(get_server_state());
        exit(EXIT_FAILURE);
      }
    }
    break;
  }
  print_server_msg("Process %d finished ", ret);
  return ret;
}

static void reset_after_game(ServerState* state) {
  // wait for the client handlers and broadcaster to finish
  // safe_waitpid(state->client_handler_pids[0], NULL, 0);
  // safe_waitpid(state->client_handler_pids[1], NULL, 0);
  safe_waitpid(state->broadcaster_pid, NULL, 0);

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
  return ret;
}

ssize_t safe_read(int fd, void *buf, size_t count) {
  ssize_t ret;
  while (1) {
    ret = read(fd, buf, count);
    if (ret < 0) {
      if (errno == EINTR) {
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
