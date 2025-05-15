#include "broadcaster.h"

/**
 * PRE:  arg1: an fd for the read end of the pipe.
 *       arg2: a pointer to the ServerState structure.
 * POST: Continuously reads messages from the pipe and broadcasts them to all connected clients.
 *       Uses the ServerState structure to access client sockets.
 *       On failure, displays an error message and exits the program.
 */
static void run_broadcaster(void *arg0, void *arg1);

static void run_broadcaster(void *arg0, void *arg1) {
  FileDescriptor pipe_fd = (int)(intptr_t)arg0;
  ServerState* state = (ServerState*)arg1;
  union Message msg;

  while (1) {
    ssize_t ret = safe_read(pipe_fd, &msg, sizeof(union Message), state);
    if (!ret) {
      break;
    }

    for (int i = 0; i < state->clients_connected; i++) {
      int sockfd = state->client_sockets[i];
      ssize_t sent = send(sockfd, &msg, sizeof(union Message), 0);
    }
    if (msg.msgt == GAME_OVER) {
      if (msg.game_over.winner == 1) {
        print_server_msg("Game over: Player 1 wins");
      } else if (msg.game_over.winner == 2) {
        print_server_msg("Game over: Player 2 wins");
      } else {
        print_server_msg("Game over: Draw");
      }
      break; // Game over
    }
  }

  close(pipe_fd);
}

void launch_broadcaster(ServerState* state, const char* map_path) {
  int pipe_fds[2];
  spipe(pipe_fds);

  FileDescriptor fd_read_broad = pipe_fds[0];
  FileDescriptor fd_write_broad = pipe_fds[1];

  state->broadcaster_pipe = fd_write_broad;

  state->broadcaster_pid = fork_and_run2(run_broadcaster, (void*)(intptr_t)fd_read_broad, (void*)state);
  close(fd_read_broad);
  print_server_msg("Broadcaster launched");

  int fd_map = sopen(map_path, O_RDONLY, 0);
  load_map(fd_map, fd_write_broad, state->shm_ptr);
  close(fd_map);
  print_server_msg("Map loaded");
}
