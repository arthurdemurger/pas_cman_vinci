#include "server_network.h"

volatile sig_atomic_t timeout_flag = 0;

/**
 * PRE:  sig: the signal number (expected to be SIGALRM).
 * POST: If one client is connected, closes the client's socket and resets the server state.
 *       Displays a message indicating the client was disconnected due to timeout.
 */
static void timeout_handler(int sig);

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

void accept_clients(ServerState *state) {
  signal(SIGALRM, timeout_handler);

  while (state->clients_connected < MAX_PLAYERS) {
      if (state->clients_connected == 1) {
          alarm(30); // Timeout pour le 2e client
      }

      int client_sock = saccept(state->server_socket);
      alarm(0);

      if (timeout_flag) {
        close(state->client_sockets[0]);
        print_server_msg("Client [1] déconnecté (timeout)");
        state->clients_connected = 0;
        timeout_flag = 0;
        continue;
      }

      if (state->clients_connected < MAX_PLAYERS) {
          uint8_t response = INSCRIPTION_OK;
          send(client_sock, &response, sizeof(response), 0);
          state->client_sockets[state->clients_connected++] = client_sock;
      } else {
          uint8_t response = INSCRIPTION_KO;
          send(client_sock, &response, sizeof(response), 0);
          close(client_sock);
      }

      print_server_msg("Client [%d] connecté", state->clients_connected);
  }

  print_server_msg("All clients connected");
}

