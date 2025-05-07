#include "client_handler.h"

/**
 * PRE:  arg: a pointer to a ClientHandlerArgs structure containing the socket file descriptor,
 *            player identifier, and broadcaster pipe.
 * POST: Handles client commands in a loop, processes user commands, and communicates with the game state.
 *       Exits when the client disconnects or the game finishes.
 * RES:  None. The function exits the process on completion.
 */
static void run_client_handler(void* arg);

static void run_client_handler(void* arg) {
  ClientHandlerArgs* args = (ClientHandlerArgs*) arg;
  ServerState* state = get_server_state();

  enum Direction dir;

  // Loop to receive commands from the client
  // and process them until the client disconnects or the game finishes
  while (1) {
    ssize_t r = safe_recv(args->sockfd, &dir, sizeof(enum Direction), 0);
    if (!r) {
      break; // client disconnected
    }

    print_server_msg("Received command from player %d: %c", args->player, dir);
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

    state->client_handler_pids[i] = fork_and_run1(run_client_handler, &args);
  }
  print_server_msg("Client handlers launched");

  // Notify players of their registration
  send_registered(1, state->client_sockets[0]);
  send_registered(2, state->client_sockets[1]);
}

