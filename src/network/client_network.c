#include "client_network.h"

int init_socket_client(int serverPort)
{
    // socket creation
    int sockfd = ssocket();

    // prepare socket to connect
    sconnect(SERVER_IP, serverPort, sockfd); // args are: serverIP, serverPort, sockfd

    return sockfd; // return socket file descriptor
}
