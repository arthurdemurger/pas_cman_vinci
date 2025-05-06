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
