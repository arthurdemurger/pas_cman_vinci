#include "broadcaster.h"

/**
 * PRE:  arg: a pointer to a structure containing the necessary data for the broadcaster,
 *            including the pipe read file descriptor and any other required context.
 * POST: Continuously reads messages from the pipe and broadcasts them to all connected clients.
 *       Uses the ServerState structure to access client sockets.
 *       On failure, displays an error message and exits the program.
 */
static void run_broadcaster(void *arg);

static void run_broadcaster(void *arg) {
  FileDescriptor pipe_fd = (int)(intptr_t)arg;
  ServerState* state = get_server_state();
  union Message msg;

  while (1) {
    ssize_t ret = sread(pipe_fd, &msg, sizeof(union Message));
    if (!ret) {
      print_server_msg("Pipe closed");
      break; // Pipe closed
    }

    for (int i = 0; i < state->clients_connected; i++) {
      int sockfd = state->client_sockets[i];
      ssize_t sent = send(sockfd, &msg, sizeof(union Message), 0);
    }
    if (msg.msgt == GAME_OVER) {
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

  state->broadcaster_pid = fork_and_run1(run_broadcaster, (void*)(intptr_t)fd_read_broad);
  close(fd_read_broad);
  print_server_msg("Broadcaster launched");

  int fd_map = sopen(map_path, O_RDONLY, 0);
  load_map(fd_map, fd_write_broad, state->shm_ptr);
  close(fd_map);
  print_server_msg("Map loaded");
}
