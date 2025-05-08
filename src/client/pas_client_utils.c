#include "pas_client_utils.h"

void cleanup(ClientState *client_state) {
    if (client_state->sock_fd != -1) {
      sclose(client_state->sock_fd);
      client_state->sock_fd = -1;
    }
    if (client_state->ui_pipe[0] != -1) {
      sclose(client_state->ui_pipe[0]);
      client_state->ui_pipe[0] = -1;
    }
      if (client_state->ui_pipe[1] != -1) {
      sclose(client_state->ui_pipe[1]);
      client_state->ui_pipe[1] = -1;
    }
    print_client_msg("Client resources cleaned up");
}

void print_client_msg(const char *format, ...) {
  va_list args;
  va_start(args, format);
  colorOn(1, BLUE_TEXT);
  printf("[CLIENT] ");
  colorOff();
  vprintf(format, args);
  printf("\n");
  va_end(args);
}

enum Direction char_to_direction(char c) {
  switch (c) {
      case '<':  return LEFT;
      case '>':  return RIGHT;
      case '^':  return UP;
      case 'v':  return DOWN;
      case '\0': return END_FILE;
      default:
          return -1;
  }
}
