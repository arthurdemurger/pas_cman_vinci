#include "server_network.h"

// Function to initialize the socket server and bind it to a port
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
  printf("Server started on %s:%d\n", SERVER_IP, state->server_port);
}

void timeout_handler(int sig) {
  ServerState* state = get_server_state();

  if (state && state->clients_connected == 1) {
    close(state->client_sockets[0]);
    printf("Client 1 disconnected due to timeout.\n");
    state->clients_connected = 0;
  }
}

// Function to accept client connections and manage the server state
void accept_clients(ServerState *state) {
  signal(SIGALRM, timeout_handler);

  while (state->clients_connected < MAX_PLAYERS) {
      if (state->clients_connected == 1) {
          alarm(30); // Timeout pour le 2e client
      }

      int client_sock = saccept(state->server_socket);
      alarm(0);

      printf("Client %d connecté.\n", state->clients_connected + 1);

      if (state->clients_connected < MAX_PLAYERS) {
          uint8_t response = INSCRIPTION_OK;
          send(client_sock, &response, sizeof(response), 0);
          state->client_sockets[state->clients_connected++] = client_sock;
      } else {
          uint8_t response = INSCRIPTION_KO;
          send(client_sock, &response, sizeof(response), 0);
          close(client_sock);
      }
  }

  printf("2 clients acceptés.\n");
}

