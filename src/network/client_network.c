#include "client_network.h"

/**
 * PRE:  serverPort: doit être un entier > 1020 et < 65535
 *     server_ip: adresse IP du serveur
 * POST: Crée une socket, connecte au serveur localhost:serverPort
 * RES:  Retourne le file descriptor de la socket connectée
 */
static int init_socket_client(char *server_ip, int serverPort);

/**
 * PRE:  state: un pointeur vers une structure ClientState
 * POST: Lit la réponse du serveur et la stocke dans le code
 * RES:  Retourne le nombre d'octets lus
 */
static ssize_t recv_server_response(ClientState *state, uint8_t *code);

static int init_socket_client(char *server_ip, int serverPort)
{
    // socket creation
    int sockfd = ssocket();

    // prepare socket to connect
    sconnect(server_ip, serverPort, sockfd); // args are: serverIP, serverPort, sockfd

    return sockfd; // return socket file descriptor
}

static ssize_t recv_server_response(ClientState *state, uint8_t *code) {
  ssize_t ret = recv(state->sock_fd, code, sizeof(uint8_t), 0);
  if (ret <= 0) {
      if (ret == 0) {
          print_client_msg("Server closed connection");
      } else {
          perror("Recv error");
      }
      cleanup(state);
      exit(EXIT_FAILURE);
  }
  return ret;
}

void connect_to_server(ClientState *client_state) {
  client_state->sock_fd = init_socket_client(client_state->server_ip, client_state->server_port);

  uint8_t code;
  // check if the server accepted the connection
  recv_server_response(client_state, &code);
  if (code == INSCRIPTION_OK) {
      print_client_msg("Inscription OK");
  } else {
      print_client_msg("Inscription KO");
      cleanup(client_state);
      exit(EXIT_FAILURE);
  }

  // check if the server is ready to launch the game
  recv_server_response(client_state, &code);
  if (code == LAUNCH_GAME) {
      print_client_msg("Launching game...");
  } else {
      print_client_msg("Error launching game");
      cleanup(client_state);
      exit(EXIT_FAILURE);
  }
}
