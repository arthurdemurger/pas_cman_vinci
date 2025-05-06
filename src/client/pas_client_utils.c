#include "pas_client_utils.h"

/**
 * Handles signals for clean termination of the client.
 *
 * PRE:
 *   - `signum` is a valid signal number (e.g., SIGINT, SIGTERM).
 * POST:
 *   - The client performs cleanup and exits gracefully.
 */
static void signal_handler(int signum);

void cleanup(ClientState *client_state) {
    if (client_state->sock_fd != -1) {
      sclose(client_state->sock_fd);
    }
    if (client_state->ui_pipe[0] != -1) {
      sclose(client_state->ui_pipe[0]);
    }
      if (client_state->ui_pipe[1] != -1) {
      sclose(client_state->ui_pipe[1]);
    }
      if (client_state->ui_pid > 0) {
      skill(client_state->ui_pid, SIGTERM);
    }
    print_client_msg("Client resources cleaned up");
}

static void signal_handler(int signum) {
    printError("Signal received, shutting down client...");
    cleanup(get_client_state());
    exit(EXIT_SUCCESS);
}

void setup_signal_handlers(void) {
    ssigaction(SIGINT, signal_handler);
    ssigaction(SIGTERM, signal_handler);
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
      case '<': return LEFT;
      case '>': return RIGHT;
      case '^': return UP;
      case 'v': return DOWN;
      default:
          return -1;
  }
}
