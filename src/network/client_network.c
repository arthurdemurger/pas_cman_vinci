#include "client_network.h"

int init_socket_client(char * serverIP, int serverPort)
{
    // socket creation
    int sockfd = ssocket();

    // prepare socket to connect
    sconnect(serverIP, serverPort, sockfd); // args are: serverIP, serverPort, sockfd

    return sockfd; // return socket file descriptor
}
