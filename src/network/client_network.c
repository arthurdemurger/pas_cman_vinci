#include "client_network.h"

/**
 * PRE:  serverPort doit être un entier > 0
 * POST: Crée une socket, connecte au serveur localhost:serverPort
 * RES:  Retourne le file descriptor de la socket connectée
 */
static int init_socket_client(int serverPort);

static int init_socket_client(int serverPort)
{
    // socket creation
    int sockfd = ssocket();

    // prepare socket to connect
    sconnect(SERVER_IP, serverPort, sockfd); // args are: serverIP, serverPort, sockfd

    return sockfd; // return socket file descriptor
}

void connect_to_server(ClientState *client_state, int server_port) {
  int sockfd = init_socket_client(server_port);

  // read response from server
  uint8_t code;
  ssize_t ret = recv(sockfd, &code, sizeof(uint8_t), 0);
  if (ret <= 0) {
      perror("Erreur lors de la lecture de la réponse du serveur");
      close(sockfd);
      exit(EXIT_FAILURE);
  }

  // check if the server accepted the connection
  if (code == INSCRIPTION_OK) {
      printf("Inscription acceptée !\n");
      client_state->sock_fd = sockfd;
  } else {
      printf("Inscription refusée.\n");
      close(sockfd);
      exit(EXIT_FAILURE);
  }
}
