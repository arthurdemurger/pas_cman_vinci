#include "pas_client.h"

int main(int ac, char **av) {
  if (ac != 2) {
    fprintf(stderr, "Usage: %s <port>\n", av[0]);
    exit(EXIT_FAILURE);
  }
  int server_port = atoi(av[1]);

  // Printf
  printf("Bienvenue dans le PAS CMAN\n");
  // printf("Pour participer entrez votre nom :\n");

  // Buffer pour l'input clavier
  // char pseudo[20];

  // Lecture clavier
  // int ret = sread(0, pseudo, 20);

  // Connexion au serveur
  int sockfd = init_socket_client(server_port);

  // Écriture du pseudo dans le socket
  // swrite(sockfd, pseudo, 20);
  sleep(30);
  return 0;
}
