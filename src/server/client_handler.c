#include "client_handler.h"

void run_client_handler(void* arg) {
  ClientHandlerArgs* args = (ClientHandlerArgs*) arg;
  ServerState* state = get_server_state();

  uint8_t dir;

  // Loop to receive commands from the client
  // and process them until the client disconnects or the game finishes
  while (1) {
    ssize_t r = recv(args->sockfd, &dir, sizeof(uint8_t), 0);
    if (r <= 0) {
      break; // client disconnected
    }

    bool finished = process_user_command(state->shm_ptr, args->player, dir, args->broadcaster_pipe);
    if (finished) {
      break;
    }
  }

  close(args->sockfd);
  free(args);
  exit(EXIT_SUCCESS);
}

void launch_client_handler(ServerState* state) {
  // Create pipes for communication
  for (int i = 0; i < 2; i++) {
    ClientHandlerArgs* args = smalloc(sizeof(ClientHandlerArgs));

    args->sockfd = state->client_sockets[i];
    args->broadcaster_pipe = state->broadcaster_pipe;
    if (i == 0) {
      args->player = PLAYER1;
    } else {
      args->player = PLAYER2;
    }

    pid_t pid = fork_and_run1(run_client_handler, args);
    checkNeg(pid, "Error forking client handler");
  }

  // Notify players after client handlers are launched
  send_registered(PLAYER1_ID, state->client_sockets[0]);
  send_registered(PLAYER2_ID, state->client_sockets[1]);
}

