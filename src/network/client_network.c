#include "client_network.h"

/**
 * PRE:  serverPort: doit être un entier > 1020 et < 65535
 *     server_ip: adresse IP du serveur
 * POST: Crée une socket, connecte au serveur localhost:serverPort
 * RES:  Retourne le file descriptor de la socket connectée
 */
static int init_socket_client(char *server_ip, int serverPort);

static int init_socket_client(char *server_ip, int serverPort)
{
    // socket creation
    int sockfd = ssocket();

    // prepare socket to connect
    sconnect(server_ip, serverPort, sockfd); // args are: serverIP, serverPort, sockfd

    return sockfd; // return socket file descriptor
}

void connect_to_server(ClientState *client_state) {
  client_state->sock_fd = init_socket_client(client_state->server_ip, client_state->server_port);

  // read response from server
  uint8_t code;
  ssize_t ret = recv(client_state->sock_fd, &code, sizeof(uint8_t), 0);
  if (ret <= 0) {
      perror("Erreur lors de la lecture de la réponse du serveur");
      cleanup(client_state);
      exit(EXIT_FAILURE);
  }

  // check if the server accepted the connection
  if (code == INSCRIPTION_OK) {
      print_client_msg("Inscription OK");
  } else {
      print_client_msg("Inscription KO");
      cleanup(client_state);
      exit(EXIT_FAILURE);
  }
}
