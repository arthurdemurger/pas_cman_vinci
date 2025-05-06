#include "pas_client.h"

static ClientState* client_state_ptr = NULL;

ClientState* get_client_state(void) {
  return client_state_ptr;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initialize client state
    ClientState client_state = {0};
    client_state_ptr = &client_state;

    // Set server port
    int server_port = atoi(argv[1]);
    check_port(server_port);

    setup_signal_handlers();

    connect_to_server(&client_state, server_port);

    // Launch the pas-cman-ipl process and set up pipes
    launch_pas_cman_ipl(&client_state);

    // Relay commands from the pas-cman-ipl process to the server
    relay_commands(&client_state);

    // Clean up resources
    cleanup(&client_state);

    // Wait for the UI process to finish
    swaitpid(client_state.ui_pid, NULL, 0);

    return EXIT_SUCCESS;
}
