#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "../shared/utils_v3.h"

#define SERVER_IP "127.0.0.1" // localhost
#define SERVER_PORT 15235  

int initSocketClient(char * serverIP, int serverPort)
{
    // socket creation
    int sockfd = ssocket();

    // prepare socket to connect
    sconnect(SERVER_IP, SERVER_PORT, sockfd); // args are: serverIP, serverPort, sockfd

    return sockfd; // return socket file descriptor  
    
}

int main(void) {

    // Printf
    printf("Bienvenue dans le PAS CMAN\n");
    printf("Pour participer entrez votre nom :\n");

    // Buffer pour l'input clavier
    char pseudo[20];

    // Lecture clavier
    int ret = sread(0, pseudo, 20);

    // Connexion au serveur
    int sockfd = initSocketClient(SERVER_IP, SERVER_PORT);
    
    // Écriture du pseudo dans le socket
    swrite(sockfd, pseudo, 20);

    return 0; 
}
