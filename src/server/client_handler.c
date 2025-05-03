#include "client_handler.h"

void launch_client_handler(ServerState* state) {

  // fork_and_run the 2 client_handler processes
  // code

  // notify players of their registration
  send_registered(PLAYER1_ID, state->client_sockets[0]);
  send_registered(PLAYER2_ID, state->client_sockets[1]);
}
