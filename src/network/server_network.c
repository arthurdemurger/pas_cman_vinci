#include "server_network.h"

volatile sig_atomic_t timeout_flag = -1;
volatile sig_atomic_t client_1_sock = -1;

/**
 * PRE:  sig: the signal number (expected to be SIGALRM).
 * POST: If one client is connected, closes the client's socket and resets the server state.
 *       Displays a message indicating the client was disconnected due to timeout.
 */
static void timeout_handler(int sig);

/**
 * PRE:  sockfd: a valid socket file descriptor.
 * POST: Accepts a connection on the socket, handling EINTR by retrying the accept call.
 *       On failure, displays an error message and exits the program.
 * RES:  Returns the accepted client socket file descriptor.
 */
static int accept_with_eintr(int sockfd);

/**
 * PRE:  state: a pointer to a ServerState structure with connected clients.
 * POST: Sends a game start signal to all connected clients.
 *       Displays an error message and exits the program on failure.
 */
static void send_clients_game_start(ServerState *state);

void init_socket_server(ServerState *state)
{
  int sockfd = ssocket();

  // Reuse the address to avoid "address already in use" error
  int optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  /* no socket error */
  sbind(state->server_port, sockfd);

  /* no bind error */
  slisten(sockfd, BACKLOG);

  state->server_socket = sockfd;
  print_server_msg("Server started on %s:%d", SERVER_IP, state->server_port);
}

static void timeout_handler(int sig) {
  timeout_flag = 1;
}

static int accept_with_eintr(int sockfd) {
  int client_sock = accept(sockfd, NULL, NULL);
  if (client_sock < 0) {
    if (errno == EINTR && timeout_flag >= 0) {
      return -1;
    }
    perror("Accept failure");
    cleanup_resources(get_server_state());
    exit(EXIT_FAILURE);
  }
  return client_sock;
}

static void send_clients_game_start(ServerState *state) {
  uint8_t response = LAUNCH_GAME;
  for (int i = 0; i < state->clients_connected; i++) {
    int n = send(state->client_sockets[i], &response, sizeof(response), 0);
    if (n < 0) {
      perror("Error sending game start signal to client");
      cleanup_resources(state);
      exit(EXIT_FAILURE);
    }
  }
}

void accept_clients(ServerState *state) {
  ssigaction(SIGALRM, timeout_handler);

  while (state->clients_connected < MAX_PLAYERS) {
      if (state->clients_connected == 1) {
        timeout_flag = 0;
        client_1_sock = state->client_sockets[0];
        alarm(5);
      }

      int client_sock = accept_with_eintr(state->server_socket);
      alarm(0);

      if (timeout_flag == 1) {
        state->clients_connected = 0;
        close(state->client_sockets[0]);
        print_server_msg("Client [1] disconnected (timeout)");
        timeout_flag = -1;
        if (get_interrupted()) {
          cleanup_resources(state);
          exit(EXIT_FAILURE);
        }
        continue;
      }

      if (state->clients_connected < MAX_PLAYERS && client_sock != -1) {
          uint8_t response = INSCRIPTION_OK;
          send(client_sock, &response, sizeof(response), 0);
          state->client_sockets[state->clients_connected++] = client_sock;
          print_server_msg("Client [%d] connected", state->clients_connected);
      } else {
          uint8_t response = INSCRIPTION_KO;
          send(client_sock, &response, sizeof(response), 0);
          close(client_sock);
      }
  }

  timeout_flag = -1;
  ssigaction(SIGALRM, NULL);
  // Notify clients that the game is starting
  send_clients_game_start(state);
  print_server_msg("All clients connected");
}

