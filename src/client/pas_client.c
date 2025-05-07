#include "pas_client.h"

static ClientState* client_state_ptr = NULL;

ClientState* get_client_state(void) {
  return client_state_ptr;
}

int main(int argc, char **argv) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage: %s <host> <port> [-test]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

    // Initialize client state
    ClientState client_state = {0};
    client_state.sock_fd = -1;
    client_state.ui_pipe[0] = -1;
    client_state.ui_pipe[1] = -1;
    client_state_ptr = &client_state;
    client_state.test_mode = (argc == 4 && strcmp(argv[3], "-test") == 0);

    // Set server port
    client_state.server_ip = argv[1];
    client_state.server_port = atoi(argv[2]);
    check_port(client_state.server_port);

    setup_signal_handlers();

    connect_to_server(&client_state);

    // Launch the pas-cman-ipl process and set up pipes
    launch_pas_cman_ipl(&client_state);

    // Relay commands from the pas-cman-ipl process to the server
    relay_commands(&client_state);

    // Clean up resources
    cleanup(&client_state);

    // Wait for the UI process to finish
    swaitpid(client_state.ui_pid, NULL, 0);
    print_client_msg("UI process terminated");
    return EXIT_SUCCESS;
}
