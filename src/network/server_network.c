#include "server_network.h"

void handle_client(int client_sock, int server_socket);

// Function to initialize the socket server and bind it to a port
int init_socket_server(int serverPort)
{
  int sockfd = ssocket();

  /* no socket error */
  sbind(serverPort, sockfd);

  /* no bind error */
  slisten(sockfd, BACKLOG);

  /* no listen error */
  return sockfd;
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
void accept_clients(int server_socket, ServerState *state) {
  while (state->clients_connected < MAX_PLAYERS) {
    if (state->clients_connected == 1) {
      alarm(30); // Set a timeout for the second client
    }

    int client_sock = saccept(server_socket);
    alarm(0); // Cancel the timeout once a client connects

    printf("Client %d connected.\n", state->clients_connected + 1);
    state->client_sockets[state->clients_connected] = client_sock;

    state->clients_connected++;
  }
}

