#include "broadcaster.h"

void run_broadcaster(void *arg) {
  printf("Broadcaster process created with PID\n");

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

pid_t launch_broadcaster(ServerState* state, const char* map_path) {
  int pipe_fds[2];
  spipe(pipe_fds);

  FileDescriptor fd_read_broad = pipe_fds[0];
  FileDescriptor fd_write_broad = pipe_fds[1];

  state->broadcaster_pipe = fd_write_broad;

  pid_t pid = fork_and_run1(run_broadcaster, (void*)(intptr_t)fd_read_broad);
  checkNeg(pid, "Fork broadcaster");

  int fd_map = sopen(map_path, O_RDONLY, 0);
  load_map(fd_map, fd_write_broad, state->shm_ptr);
  close(fd_map);
  printf("Map loaded\n");

  return pid;
}
