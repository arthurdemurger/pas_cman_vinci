#include "ui_handler.h"

void launch_pas_cman_ipl(ClientState* client_state) {
    spipe(client_state->ui_pipe);

    client_state->ui_pid = sfork();
    if (client_state->ui_pid == 0) {
        sclose(client_state->ui_pipe[0]);
        sdup2(client_state->ui_pipe[1], STDOUT_FILENO);
        sdup2(client_state->sock_fd, STDIN_FILENO);
        sclose(client_state->ui_pipe[1]);
        sexecl("./src/ui/student_kit/target/release/pas-cman-ipl", "pas-cman-ipl", NULL);
    }
    sclose(client_state->ui_pipe[1]);
    print_client_msg("pas-cman-ipl launched");
}

void relay_commands(ClientState* client_state) {
    enum Direction dir;

    while (1) {
        ssize_t n = sread(client_state->ui_pipe[0], &dir, sizeof(enum Direction));
        if (n == 0) {
            print_client_msg("UI process terminated");
            break;
        }
        if (n < 0) {
            printError("Error reading from pipe");
            break;
        }
        ssize_t sent = send(client_state->sock_fd, &dir, sizeof(enum Direction), 0);
        if (sent < 0) {
            printError("Error sending to server");
            break;
        }
    }
}
