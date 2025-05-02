#include "pas_client.h"

int main(void) {

    // Printf
    printf("Bienvenue dans le PAS CMAN\n");
    printf("Pour participer entrez votre nom :\n");

    // Buffer pour l'input clavier
    char pseudo[20];

    // Lecture clavier
    int ret = sread(0, pseudo, 20);

    // Connexion au serveur
    int sockfd = init_socket_client(SERVER_IP, SERVER_PORT);

    // Écriture du pseudo dans le socket
    // swrite(sockfd, pseudo, 20);

    return 0;
}
