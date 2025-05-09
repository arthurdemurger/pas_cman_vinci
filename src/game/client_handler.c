#include "client_handler.h"

/**
 * PRE:  arg0: a pointer to ClientHandlerArgs structure containing the socket file descriptor and player ID.
 *       arg1: a pointer to the ServerState structure containing the game state and shared memory.
 * POST: Handles client commands in a loop, processes user commands, and communicates with the game state.
 *       Exits when the client disconnects or the game finishes.
 * RES:  None. The function exits the process on completion.
 */
static void run_client_handler(void* arg0, void* arg1);

static void run_client_handler(void* arg0, void* arg1) {
  ClientHandlerArgs* args = (ClientHandlerArgs*) arg0;
  ServerState* state = (ServerState *) arg1;

  enum Direction dir;

  // Loop to receive commands from the client
  // and process them until the client disconnects or the game finishes
  while (1) {
    ssize_t r = safe_recv(args->sockfd, &dir, sizeof(enum Direction), 0, state);
    if (!r) {
      break; // client disconnected
    }

    bool finished = process_user_command(state->shm_ptr, args->player, dir, args->broadcaster_pipe);
    if (finished) {
      break;
    }
  }

  close(args->broadcaster_pipe);
  close(args->sockfd);
  exit(EXIT_SUCCESS);
}

void launch_client_handler(ServerState* state) {
  // Create pipes for communication
  for (int i = 0; i < 2; i++) {
    ClientHandlerArgs args = {0};

    args.sockfd = state->client_sockets[i];
    args.broadcaster_pipe = state->broadcaster_pipe;
    if (i == 0) {
      args.player = PLAYER1;
    } else {
      args.player = PLAYER2;
    }

    state->client_handler_pids[i] = fork_and_run2(run_client_handler, (void*) &args, (void*) state);
  }
  print_server_msg("Client handlers launched");

  // Notify players of their registration
  send_registered(1, state->client_sockets[0]);
  send_registered(2, state->client_sockets[1]);
}

