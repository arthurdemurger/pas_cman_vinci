#include "broadcaster.h"

void run_broadcaster(void *arg) {
  FileDescriptor pipe_fd = (int)(intptr_t)arg;
  ServerState* state = get_server_state();
  union Message msg;

  while (1) {
    ssize_t ret = sread(pipe_fd, &msg, sizeof(union Message));

    for (int i = 0; i < state->clients_connected; i++) {
      int sockfd = state->client_sockets[i];
      ssize_t sent = send(sockfd, &msg, sizeof(union Message), 0);
    }
  }

  close(pipe_fd);
}
