#include "pas_client.h"

int main(int ac, char **av) {
  if (ac != 2) {
    fprintf(stderr, "Usage: %s <port>\n", av[0]);
    exit(EXIT_FAILURE);
  }
  int server_port = atoi(av[1]);

  int sock_fd = connect_to_server(server_port);

  // launch pas-man-ipl here

  sleep(30);
}
